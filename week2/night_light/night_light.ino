

// Pin Definitions
const int LED_PIN = 9;       // Must be a PWM pin (indicated by '~' on Arduino)
const int BUTTON_PIN = 2;    // Button pin for mode toggle

// Operational Modes
enum LightMode {
  SLOW_BREATH,   // Mode 0: 3-second cycle
  FAST_PULSE,    // Mode 1: 0.5-second cycle
  SOS_PATTERN    // Mode 2: Distress flash
};

LightMode currentMode = SLOW_BREATH;

// Non-blocking Timing Variables
unsigned long lastModeChange = 0;
bool lastButtonState = HIGH;

// Variables for SOS Mode tracking
unsigned long sosStartTime = 0;
int sosStep = 0;
const int sosTimings[] = {
  200, 200, 200, 200, 200, 200, // S: 3 short flashes (on 200ms, off 200ms)
  600, 200, 600, 200, 600, 200, // O: 3 long flashes  (on 600ms, off 200ms)
  200, 200, 200, 200, 200, 1000 // S: 3 short flashes (on 200ms, off 1000ms rest)
};
const int SOS_STEPS_TOTAL = 18;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Uses internal pull-up resistor
  
  // Print initial state
  logMode();
}

void loop() {
  unsigned long currentTime = millis();
  
  // --- 1. Button Press & State Detection ---
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // Detect falling edge (Unpressed to Pressed transition)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    // Software debounce check (50ms window)
    if (currentTime - lastModeChange > 50) {
      // Rotate through modes: 0 -> 1 -> 2 -> 0
      if (currentMode == SLOW_BREATH) currentMode = FAST_PULSE;
      else if (currentMode == FAST_PULSE) currentMode = SOS_PATTERN;
      else if (currentMode == SOS_PATTERN) currentMode = SLOW_BREATH;
      
      sosStartTime = currentTime; // Reset SOS progression if entering mode 2
      sosStep = 0;
      
      logMode();
      lastModeChange = currentTime;
    }
  }
  lastButtonState = currentButtonState;

  // --- 2. Mode Execution Matrix ---
  if (currentMode == SLOW_BREATH) {
    // 3-second cycle = 3000ms
    // Converts time into a smooth sine wave cycle bouncing between 0 and 255
    float period = 3000.0;
    float angle = (currentTime / period) * 2.0 * PI;
    int brightness = (sin(angle) + 1.0) * 127.5; // Map sin (-1 to 1) to PWM (0 to 255)
    analogWrite(LED_PIN, brightness);
  } 
  else if (currentMode == FAST_PULSE) {
    // 0.5-second cycle = 500ms
    float period = 500.0;
    float angle = (currentTime / period) * 2.0 * PI;
    int brightness = (sin(angle) + 1.0) * 127.5;
    analogWrite(LED_PIN, brightness);
  } 
  else if (currentMode == SOS_PATTERN) {
    // Calculate how much time has passed inside the current flash segment
    unsigned long timeInStep = currentTime - sosStartTime;
    
    if (timeInStep >= sosTimings[sosStep]) {
      sosStep = (sosStep + 1) % SOS_STEPS_TOTAL; // Move to next step loop
      sosStartTime = currentTime;
    }
    
    // Even steps are ON (0, 2, 4 = S | 6, 8, 10 = O | 12, 14, 16 = S)
    // Odd steps are OFF spacing periods
    if (sosStep % 2 == 0) {
      analogWrite(LED_PIN, 255); // Full brightness
    } else {
      analogWrite(LED_PIN, 0);   // Dark
    }
  }
}

// Log status to Serial monitor
void logMode() {
  Serial.print("[Mode Update] Current State: ");
  if (currentMode == SLOW_BREATH) Serial.println("1 - SLOW BREATH (3s Cycle)");
  else if (currentMode == FAST_PULSE) Serial.println("2 - FAST PULSE (0.5s Cycle)");
  else if (currentMode == SOS_PATTERN) Serial.println("3 - EMERGENCY SOS PATTERN");
}
