#include "DHT.h"

#define DHTPIN 2     
#define DHTTYPE DHT22

const int LED_RED = 8;
const int LED_GREEN = 9;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  dht.begin();
  
  // CSV Header
  Serial.println("timestamp_ms,temp_C,temp_F,humidity_pct");
}

void loop() {
  delay(2000); // Wait 2 seconds between measurements

  float h = dht.readHumidity();
  float c = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(c) || isnan(f)) return;

  // Print CSV values
  Serial.print(millis()); Serial.print(",");
  Serial.print(c); Serial.print(",");
  Serial.print(f); Serial.print(",");
  Serial.println(h);

  // Threshold alerts
  if (c > 35.0 || h > 80.0) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}

