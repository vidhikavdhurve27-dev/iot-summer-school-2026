const int LDR_PIN = A0;
const int PIR_PIN = 4;
const int LED_PWM = 9;

const int LDR_THRESHOLD = 400; // Value below this means dark
unsigned long motionStartTime = 0;
bool motionActive = false;

void logEvent(String desc) {
  // Simple simulator uptime tracking timestamp
  unsigned long s = millis() / 1000;
  int h = s / 3600;
  int m = (s % 3600) / 60;
  int sec = s % 60;
  
  Serial.print("[");
  if(h<10) Serial.print("0"); Serial.print(h); Serial.print(":");
  if(m<10) Serial.print("0"); Serial.print(m); Serial.print(":");
  if(sec<10) Serial.print("0"); Serial.print(sec);
  Serial.print("] EVENT: ");
  Serial.println(desc);
}

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PWM, OUTPUT);
  logEvent("System initialized.");
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  bool isDark = (ldrValue < LDR_THRESHOLD);

  if (!isDark) {
    analogWrite(LED_PWM, 0); // Daylight: OFF
    if (motionActive) {
      motionActive = false;
      logEvent("Daylight detected. Disabling street light.");
    }
  } 
  else {
    // Night Mode
    bool motion = (digitalRead(PIR_PIN) == HIGH);
    
    if (motion) {
      if (!motionActive || (analogRead(LED_PWM) != 255)) {
        logEvent("Motion detected at night. Scaling light to 100%.");
      }
      motionActive = true;
      motionStartTime = millis();
      analogWrite(LED_PWM, 255); // 100% Brightness
    } 
    else {
      if (motionActive && (millis() - motionStartTime >= 30000)) {
        logEvent("30s vacancy timeout reached. Dimming light to 20%.");
        analogWrite(LED_PWM, 51); // 20% of 255 is ~51
        motionActive = false; 
      }
    }
  }
  delay(100);
}
