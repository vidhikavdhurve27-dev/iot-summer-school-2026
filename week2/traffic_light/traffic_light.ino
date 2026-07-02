/**
 * traffic_light.ino
 */

const int PIN_RED = 11;
const int PIN_YELLOW = 12;
const int PIN_GREEN = 13;
const int PIN_BUTTON = 7;

unsigned long stateStartTime = 0;
unsigned long currentDuration = 5000; // Starts on Red (5000ms)
int currentState = 0; // 0: RED, 1: YELLOW, 2: GREEN

void setLightState(int state) {
  currentState = state;
  stateStartTime = millis();
  
  digitalWrite(PIN_RED, currentState == 0 ? HIGH : LOW);
  digitalWrite(PIN_YELLOW, currentState == 1 ? HIGH : LOW);
  digitalWrite(PIN_GREEN, currentState == 2 ? HIGH : LOW);
  
  Serial.print("["); Serial.print(stateStartTime); Serial.print(" ms] State: ");
  if (currentState == 0) { Serial.println("RED"); currentDuration = 5000; }
  else if (currentState == 1) { Serial.println("YELLOW"); currentDuration = 2000; }
  else if (currentState == 2) { Serial.println("GREEN"); currentDuration = 4000; }
}

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP); // Button pin pulled HIGH by default
  Serial.begin(9600);
  setLightState(0); // Start on Red
}

void loop() {
  // Check for pedestrian button override (Active LOW when pressed)
  if (digitalWrite(PIN_BUTTON) == LOW) {
    if (currentState != 0 || (currentState == 0 && currentDuration != 8000)) {
      Serial.print("["); Serial.print(millis()); Serial.println(" ms] WARNING: Pedestrian Override Triggered!");
      currentState = 0;
      stateStartTime = millis();
      currentDuration = 8000; // Hold Red for 8 seconds
      digitalWrite(PIN_RED, HIGH);
      digitalWrite(PIN_YELLOW, LOW);
      digitalWrite(PIN_GREEN, LOW);
      delay(250); // Soft debounce
    }
  }

  // Handle standard timed transitions
  if (millis() - stateStartTime >= currentDuration) {
    int nextState = (currentState + 1) % 3;
    setLightState(nextState);
  }
}
