/**
 * serial_interface.ino
 */

const int LED_PIN = LED_BUILTIN; // Pin 13
int totalBlinksExecuted = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  Serial.println("System online. Enter command:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Strip carriage returns or invisible spaces
    
    if (input.length() == 0) return;

    if (input == "LED_ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("SUCCESS: Built-in LED activated.");
    } 
    else if (input == "LED_OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("SUCCESS: Built-in LED deactivated.");
    } 
    else if (input.startsWith("BLINK_") && input.length() == 7) {
      char numChar = input.charAt(6);
      if (numChar >= '1' && numChar <= '9') {
        int blinkTimes = numChar - '0';
        Serial.print("SUCCESS: Blinking "); Serial.print(blinkTimes); Serial.println(" times.");
        
        for (int i = 0; i < blinkTimes; i++) {
          digitalWrite(LED_PIN, HIGH);
          delay(200);
          digitalWrite(LED_PIN, LOW);
          delay(200);
          totalBlinksExecuted++;
        }
      } else {
        Serial.println("ERROR: Unknown command");
      }
    } 
    else if (input == "STATUS") {
      Serial.print("STATUS -> LED_PIN State: ");
      Serial.print(digitalRead(LED_PIN) == HIGH ? "ON" : "OFF");
      Serial.print(" | Total Historical Blinks: ");
      Serial.println(totalBlinksExecuted);
    } 
    else if (input == "RESET") {
      totalBlinksExecuted = 0;
      Serial.println("SUCCESS: Historical blink counter cleared.");
    } 
    else {
      Serial.println("ERROR: Unknown command");
    }
  }
}
