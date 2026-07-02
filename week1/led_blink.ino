
/**
  * File led_blink.ino
  * Author: Vidhika Vilas Dhurve
  * Date: 02 July 2026
  * Brief: Led blinking controller using a potentiometer and serial tracking
  * Fixes #1
  */

int blinkCount = 0;
const int potPin = A0;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  int potValue = analogRead(potPin); // Read potentiometer
  
  digitalWrite(13, HIGH);
  delay(potValue); // 500ms interval
  digitalWrite(13, LOW);
  delay(potValue);
  
  blinkCount++;
  Serial.print("Blink Count: ");
  Serial.println(blinkCount);
    
}
