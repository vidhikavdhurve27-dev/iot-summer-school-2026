/**
 * night_light.ino
 */

const int PIN_LED = 9;       // Must be a PWM pin (~9)
const int PIN_BUTTON = 2;    // Button pin

int currentMode = 0;         // 0: Slow, 1: Fast, 2: SOS
unsigned long modeStartTime = 0;
bool lastButtonState = HIGH;

// SOS non-blocking configuration timings (in ms)
const int SOS_TIMINGS[] = {
  150, 150, 150, 150, 150, 450, // S: 3 Short blinks (ON/OFF cycles)
  450, 150, 450, 150, 450, 450, // O: 3 Long blinks
  150, 150, 150, 150, 150, 1000 // S: 3 Short blinks + word delay
};
int sosStep = 0;
unsigned long lastSosSwitch = 0;

void printMode() {
  Serial.print("Mode Switched: ");
  if (currentMode == 0) Serial.println("1 - Slow Breathing (3s)");
  else if (currentMode == 1) Serial.println("2 - Fast Pulse (0.5s)");
  else if (currentMode == 2) Serial.println("3 - SOS Pattern");
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
  printMode();
}

void loop() {
  // 1. Check Button Input with edge detection
  bool currentButtonState = digitalRead(PIN_BUTTON);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    currentMode = (currentMode + 1) % 3;
    modeStartTime = millis();
    sosStep = 0;
    lastSosSwitch = millis();
    printMode();
    delay(50); // Simple mechanical debounce debouncer
  }
  lastButtonState = currentButtonState;

  // 2. Execute Selected Pattern Mode
  unsigned long elapsed = millis() - modeStartTime;

  if (currentMode == 0) {
    // Mode 1: Slow Breathing (3000ms cycle)
    unsigned long progress = elapsed % 3000;
    int brightness = (progress < 1500) ? map(progress, 0, 1500, 0, 255) 
                                       : map(progress, 1500, 3000, 255, 0);
    analogWrite(PIN_LED, brightness);
  } 
  else if (currentMode == 1) {
    // Mode 2: Fast Pulse (500ms cycle)
    unsigned long progress = elapsed % 500;
    int brightness = (progress < 250) ? map(progress, 0, 250, 0, 255) 
                                      : map(progress, 250, 500, 255, 0);
    analogWrite(PIN_LED, brightness);
  } 
  else if (currentMode == 2) {
    // Mode 3: SOS (... --- ...) Pattern Engine
    if (millis() - lastSosSwitch >= SOS_TIMINGS[sosStep]) {
      sosStep = (sosStep + 1) % 18;
      lastSosSwitch = millis();
    }
    // Even steps are ON (0,2,4, 6,8,10, 12,14,16)
    int ledState = (sosStep % 2 == 0) ? HIGH : LOW;
    digitalWrite(PIN_LED, ledState);
  }
}
