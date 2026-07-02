int blinkCount= 0;

void setup() {
  pinMode(13, OUTPUT);
}
void loop() {
  digitalWrite(13, HIGH);
  delay(500); // 500ms interval
  digitalWrite(13, LOW);
  delay(500);
  blinkCount++;
  Serial.print("Blink Count: ");
  Serial.println(blinkCount);
    
}
