const int TRIG_PIN = 5;
const int ECHO_PIN = 6;
const int LED_YEL = 10;
const int LED_RED = 11;
const int PIN_BUZZER = 3;

unsigned long lastActionTime = 0;
unsigned long lastSensorTime = 0;
bool toggleState = false;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_YEL, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  // Read sensor every 100ms
  if (now - lastSensorTime >= 100) {
    lastSensorTime = now;
    
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
    float distance = (duration * 0.034) / 2.0;

    if (distance == 0 || distance > 50) {
      Serial.println("SAFE");
      digitalWrite(LED_YEL, LOW);
      digitalWrite(LED_RED, LOW);
      noTone(PIN_BUZZER);
    } 
    else if (distance >= 20 && distance <= 50) {
      digitalWrite(LED_YEL, HIGH);
      digitalWrite(LED_RED, LOW);
      if (now - lastActionTime >= 500) {
        lastActionTime = now;
        toggleState = !toggleState;
        if (toggleState) tone(PIN_BUZZER, 1000); else noTone(PIN_BUZZER);
      }
    } 
    else if (distance >= 10 && distance < 20) {
      digitalWrite(LED_YEL, LOW);
      digitalWrite(LED_RED, HIGH);
      if (now - lastActionTime >= 200) {
        lastActionTime = now;
        toggleState = !toggleState;
        if (toggleState) tone(PIN_BUZZER, 1500); else noTone(PIN_BUZZER);
      }
    } 
    else if (distance < 10) {
      noTone(PIN_BUZZER); // Reset tone first
      tone(PIN_BUZZER, 2000); // Continuous tone
      if (now - lastActionTime >= 50) {
        lastActionTime = now;
        toggleState = !toggleState;
        digitalWrite(LED_YEL, toggleState);
        digitalWrite(LED_RED, !toggleState);
      }
    }
  }
}
