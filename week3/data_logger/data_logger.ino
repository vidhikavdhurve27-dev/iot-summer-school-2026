#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

const int LDR_PIN = A0;
const int TRIG_PIN = 5;
const int ECHO_PIN = 6;

DHT dht(DHTPIN, DHTTYPE);
unsigned long lastLogTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht.begin();
}

void loop() {
  unsigned long now = millis();
  
  if (now - lastLogTime >= 5000) {
    lastLogTime = now;

    // Read Sensors
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    int rawLDR = analogRead(LDR_PIN);
    int lightPct = map(rawLDR, 0, 1023, 0, 100);
    String lightDesc = (lightPct > 70) ? "Bright" : (lightPct > 30) ? "Normal" : "Dark";

    digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    int distance = (duration * 0.034) / 2;

    // Print Log Structure
    Serial.println("=== SENSOR LOG ===");
    Serial.print("Time      : "); Serial.print(now); Serial.println(" ms");
    Serial.print("Temp      : "); Serial.print(t, 1); Serial.print(" C | Humidity: "); Serial.print(h, 0); Serial.println("%");
    Serial.print("Light     : "); Serial.print(lightPct); Serial.print("% ("); Serial.print(lightDesc); Serial.println(")");
    Serial.print("Distance  : "); Serial.print(distance); Serial.println(" cm");
    Serial.println("==================");
  }
}
