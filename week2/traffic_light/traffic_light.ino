/**
 * traffic_light.ino
 */

// Pin Definitions
const int RED_PIN    = 13; // Pin for Red LED
const int YELLOW_PIN = 12; // Pin for Yellow LED
const int GREEN_PIN  = 11; // Pin for Green LED
const int BUTTON_PIN = 7;  // Pin for Pedestrian Button

// Traffic Light States
enum LightState {
  STATE_RED,
  STATE_YELLOW,
  STATE_GREEN
};

LightState currentState = STATE_RED; // Initial light state
unsigned long stateStartTime = 0;    // Tracks when the current state started
unsigned long stateDuration = 5000;  // Initial duration for RED (5 seconds)

// Tracking button state to prevent multi-triggering on single press
bool lastButtonState = HIGH; 

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Configure LED pins as outputs
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
  // Configure button with internal pull-up resistor
  // Button reads LOW when pressed, HIGH when unpressed
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Set initial physical light state and log it
  updatePhysicalLights();
  logState();
  stateStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // 1. Check Pedestrian Button (Detect falling edge: HIGH to LOW transition)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    // Debounce check: ensure we aren't overriding an already active pedestrian block
    if (currentState != STATE_RED || stateDuration != 8000) {
      Serial.print("[");
      Serial.print(currentTime);
      Serial.println(" ms] PEDESTRIAN BUTTON PRESSED! Forcing RED state.");
      
      currentState = STATE_RED;
      stateDuration = 8000; // Hold RED for 8 seconds as requested
      stateStartTime = currentTime;
      
      updatePhysicalLights();
      logState();
    }
  }
  lastButtonState = currentButtonState; // Save button state for next iteration

  // 2. Manage regular time-based state transitions
  if (currentTime - stateStartTime >= stateDuration) {
    // Advance to next state based on standard cycle
    switch (currentState) {
      case STATE_RED:
        currentState = STATE_YELLOW;
        stateDuration = 2000; // Yellow for 2 seconds
        break;
        
      case STATE_YELLOW:
        currentState = STATE_GREEN;
        stateDuration = 4000; // Green for 4 seconds
        break;
        
      case STATE_GREEN:
        currentState = STATE_RED;
        stateDuration = 5000; // Return to standard Red for 5 seconds
        break;
    }
    
    stateStartTime = currentTime; // Reset state timer
    updatePhysicalLights();       // Apply hardware output
    logState();                   // Output to Serial Monitor
  }
}

// Helper function to update the physical state of the LEDs
void updatePhysicalLights() {
  switch (currentState) {
    case STATE_RED:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      break;
    case STATE_YELLOW:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      break;
    case STATE_GREEN:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      break;
  }
}

// Helper function to print the log to the Serial Monitor
void logState() {
  Serial.print("[Timestamp: ");
  Serial.print(millis());
  Serial.print(" ms] Current Light State: ");
  
  switch (currentState) {
    case STATE_RED:
      Serial.print("RED (Duration: ");
      Serial.print(stateDuration / 1000);
      Serial.println("s)");
      break;
    case STATE_YELLOW:
      Serial.println("YELLOW (Duration: 2s)");
      break;
    case STATE_GREEN:
      Serial.println("GREEN (Duration: 4s)");
      break;
  }
}
// Pin Definitions
const int RED_PIN    = 13; // Pin for Red LED
const int YELLOW_PIN = 12; // Pin for Yellow LED
const int GREEN_PIN  = 11; // Pin for Green LED
const int BUTTON_PIN = 7;  // Pin for Pedestrian Button

// Traffic Light States
enum LightState {
  STATE_RED,
  STATE_YELLOW,
  STATE_GREEN
};

LightState currentState = STATE_RED; // Initial light state
unsigned long stateStartTime = 0;    // Tracks when the current state started
unsigned long stateDuration = 5000;  // Initial duration for RED (5 seconds)

// Tracking button state to prevent multi-triggering on single press
bool lastButtonState = HIGH; 

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Configure LED pins as outputs
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
  // Configure button with internal pull-up resistor
  // Button reads LOW when pressed, HIGH when unpressed
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Set initial physical light state and log it
  updatePhysicalLights();
  logState();
  stateStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // 1. Check Pedestrian Button (Detect falling edge: HIGH to LOW transition)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    // Debounce check: ensure we aren't overriding an already active pedestrian block
    if (currentState != STATE_RED || stateDuration != 8000) {
      Serial.print("[");
      Serial.print(currentTime);
      Serial.println(" ms] PEDESTRIAN BUTTON PRESSED! Forcing RED state.");
      
      currentState = STATE_RED;
      stateDuration = 8000; // Hold RED for 8 seconds as requested
      stateStartTime = currentTime;
      
      updatePhysicalLights();
      logState();
    }
  }
  lastButtonState = currentButtonState; // Save button state for next iteration

  // 2. Manage regular time-based state transitions
  if (currentTime - stateStartTime >= stateDuration) {
    // Advance to next state based on standard cycle
    switch (currentState) {
      case STATE_RED:
        currentState = STATE_YELLOW;
        stateDuration = 2000; // Yellow for 2 seconds
        break;
        
      case STATE_YELLOW:
        currentState = STATE_GREEN;
        stateDuration = 4000; // Green for 4 seconds
        break;
        
      case STATE_GREEN:
        currentState = STATE_RED;
        stateDuration = 5000; // Return to standard Red for 5 seconds
        break;
    }
    
    stateStartTime = currentTime; // Reset state timer
    updatePhysicalLights();       // Apply hardware output
    logState();                   // Output to Serial Monitor
  }
}

// Helper function to update the physical state of the LEDs
void updatePhysicalLights() {
  switch (currentState) {
    case STATE_RED:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      break;
    case STATE_YELLOW:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      break;
    case STATE_GREEN:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      break;
  }
}

// Helper function to print the log to the Serial Monitor
void logState() {
  Serial.print("[Timestamp: ");
  Serial.print(millis());
  Serial.print(" ms] Current Light State: ");
  
  switch (currentState) {
    case STATE_RED:
      Serial.print("RED (Duration: ");
      Serial.print(stateDuration / 1000);
      Serial.println("s)");
      break;
    case STATE_YELLOW:
      Serial.println("YELLOW (Duration: 2s)");
      break;
    case STATE_GREEN:
      Serial.println("GREEN (Duration: 4s)");
      break;
  }
}

