/**
 * @file motor_control.ino
 * @brief DC Motor speed and direction control via L298N H-Bridge.
 */

// L298N Pin Configurations
const int PIN_ENA = 9;  // Must be a PWM pin (~9)
const int PIN_IN1 = 4;
const int PIN_IN2 = 5;

// Input Pins
const int POT_PIN = A0;
const int BTN_DIR = 2;
const int BTN_STOP = 3;

// State Variables
bool isRunning = true;
bool isForward = true;
bool lastDirBtnState = HIGH;
bool lastStopBtnState = HIGH;
unsigned long lastLogTime = 0;

void updateMotor() {
  if (!isRunning) {
    // Stop condition (Brake)
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, 0);
    return;
  }

  // Read speed from potentiometer (0-1023 -> 0-255)
  int potVal = analogRead(POT_PIN);
  int pwmValue = map(potVal, 0, 1023, 0, 255);
  analogWrite(PIN_ENA, pwmValue);

  // Set direction
  if (isForward) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
  }
}

void logTelemetry() {
  if (millis() - lastLogTime >= 500) { // Log twice per second
    lastLogTime = millis();
    
    int potVal = analogRead(POT_PIN);
    int speedPct = map(potVal, 0, 1023, 0, 100);

    Serial.print("State: "); Serial.print(isRunning ? "RUNNING" : "STOPPED");
    Serial.print(" | Direction: "); Serial.print(isForward ? "FORWARD" : "REVERSE");
    Serial.print(" | Speed: "); Serial.print(speedPct); Serial.println("%");
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  
  pinMode(BTN_DIR, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
  
  Serial.println("L298N Motor Interface Initialized.");
}

void loop() {
  bool currentDirBtn = digitalRead(BTN_DIR);
  bool currentStopBtn = digitalRead(BTN_STOP);

  // Edge detection for Direction Button
  if (lastDirBtnState == HIGH && currentDirBtn == LOW) {
    isForward = !isForward;
    delay(50); // Debounce
  }

  // Edge detection for Stop/Start Button
  if (lastStopBtnState == HIGH && currentStopBtn == LOW) {
    isRunning = !isRunning;
    delay(50); // Debounce
  }

  lastDirBtnState = currentDirBtn;
  lastStopBtnState = currentStopBtn;

  updateMotor();
  logTelemetry();
}
