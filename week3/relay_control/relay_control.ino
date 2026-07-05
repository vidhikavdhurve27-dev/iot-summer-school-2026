#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

const int PIN_RELAY = 8;
const int PIN_BUTTON = 7;

DHT dht(DHTPIN, DHTTYPE);

bool relayState = false;
bool manualOverride = false;
bool lastButtonState = HIGH;
unsigned long lastSensorReadTime = 0;

void logStatus(float temp, String source) {
  Serial.print("["); Serial.print(millis() / 1000); Serial.print("s] ");
  Serial.print("Temp: "); Serial.print(temp, 1); Serial.print("°C | ");
  Serial.print("Relay (AC): "); Serial.print(relayState ? "ON" : "OFF");
  Serial.print(" | Mode: "); Serial.println(source);
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  digitalWrite(PIN_RELAY, LOW); // Start with AC off
  dht.begin();
  Serial.println("Climate Controller System Online.");
}

void loop() {
  unsigned long now = millis();
  bool currentButtonState = digitalRead(PIN_BUTTON);

  // 1. Handle Manual Override Button (Edge Detection)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    manualOverride = !manualOverride; // Toggle override mode
    relayState = !relayState;         // Flip the current relay state
    
    digitalWrite(PIN_RELAY, relayState ? HIGH : LOW);
    
    float t = dht.readTemperature();
    logStatus(isnan(t) ? 0.0 : t, manualOverride ? "MANUAL OVERRIDE ACTIVE" : "RETURN TO AUTO");
    delay(50); // Soft debounce
  }
  lastButtonState = currentButtonState;

  // 2. Read Sensor & Process Automated Hysteresis (Every 2 seconds)
  if (now - lastSensorReadTime >= 2000) {
    lastSensorReadTime = now;
    
    float t = dht.readTemperature();
    if (isnan(t)) return; // Skip loop if sensor reads fail

    if (!manualOverride) {
      bool previousState = relayState;
      
      // Hysteresis Logic
      if (t > 32.0) {
        relayState = true;  // Turn AC ON
      } else if (t < 28.0) {
        relayState = false; // Turn AC OFF
      }
      
      // Update hardware only if state actually changed
      if (relayState != previousState) {
        digitalWrite(PIN_RELAY, relayState ? HIGH : LOW);
        logStatus(t, "AUTOMATIC SYSTEM SHIFT");
      } else {
        // Routine telemetry printout
        logStatus(t, "AUTO MONITORING");
      }
    } else {
      // Print status while tracking manual lock state
      logStatus(t, "MANUAL OVERRIDE HOLD");
    }
  }
}
