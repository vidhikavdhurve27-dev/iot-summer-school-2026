#include <Servo.h>

Servo myServo;
// Pin configurations
const int POT_PIN = A0;
const int BTN_PIN = 7;
const int SERVO_PIN = 9;

bool lastBtnState = HIGH;

void setup() {
  Serial.begin(9600);
  myServo.attach(SERVO_PIN);
  pinMode(BTN_PIN, INPUT_PULLUP);
}

void loop() {
  bool currentBtnState = digitalRead(BTN_PIN);
  
  // Check for button press (Active LOW)
  if (lastBtnState == HIGH && currentBtnState == LOW) {
    Serial.println("[EVENT] Button Pressed: Executing Full Sweep (0 -> 180 -> 0)");
    
    // Sweep from 0 to 180 degrees
    for (int angle = 0; angle <= 180; angle++) {
      myServo.write(angle);
      delay(15); 
    }
    // Sweep back from 180 to 0 degrees
    for (int angle = 180; angle >= 0; angle--) {
      myServo.write(angle);
      delay(15);
    }
    Serial.println("[EVENT] Sweep Completed. Returning to Potentiometer Control.");
    delay(200); // Simple mechanical debounce
  } 
  else {
    // Standard Mode: Map Potentiometer to Angle
    int potVal = analogRead(POT_PIN);
    int angle = map(potVal, 0, 1023, 0, 180);
    
    myServo.write(angle);
    
    // Log telemetry data to Serial Monitor
    Serial.print("Potentiometer: "); Serial.print(potVal);
    Serial.print(" | Servo Angle: "); Serial.print(angle);
    Serial.println(" deg");
    
    delay(100); // Smooth out the serial printing
  }
  
  lastBtnState = currentBtnState;
}

