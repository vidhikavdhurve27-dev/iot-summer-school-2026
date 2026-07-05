// Pin Definitions
const int LED_PIN = 13; // Built-in LED pin

// Variables for non-blocking blink command
bool isBlinking = false;
int blinksRemaining = 0;
int ledState = LOW;
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 300; // 300ms on, 300ms off

// Counter variables
int totalBlinksExecuted = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Print initial greeting menu
  Serial.println("--- Serial Hardware Controller Ready ---");
  Serial.println("Available commands: LED_ON, LED_OFF, BLINK_X (X: 1-9), STATUS, RESET");
}

void loop() {
  // 1. Check for incoming Serial Data
  if (Serial.available() > 0) {
    // Read string until newline character
    String inputString = Serial.readStringUntil('\n');
    inputString.trim(); // Remove hidden carriage returns (\r) or spaces

    if (inputString.length() > 0) {
      processCommand(inputString);
    }
  }

  // 2. Handle Non-Blocking Blink Execution
  if (isBlinking) {
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = currentTime;
      
      // Toggle LED state
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
        // One full blink cycle (ON + OFF) completed
        blinksRemaining--;
        totalBlinksExecuted++;
        if (blinksRemaining <= 0) {
          isBlinking = false;
          Serial.println("SUCCESS: Blink sequence finished.");
        }
      }
      digitalWrite(LED_PIN, ledState);
    }
  }
}

// Custom Command Processing Matrix
void processCommand(String command) {
  if (command == "LED_ON") {
    isBlinking = false; // Override any active blink patterns
    digitalWrite(LED_PIN, HIGH);
    Serial.println("SUCCESS: LED turned ON");
  } 
  else if (command == "LED_OFF") {
    isBlinking = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("SUCCESS: LED turned OFF");
  } 
  else if (command.startsWith("BLINK_")) {
    // Extract characters after the underscore
    String countPart = command.substring(6); 
    
    // Validate that it is exactly 1 character long and between '1' and '9'
    if (countPart.length() == 1 && countPart.charAt(0) >= '1' && countPart.charAt(0) <= '9') {
      int x = countPart.toInt();
      blinksRemaining = x;
      isBlinking = true;
      ledState = LOW; // Start clean
      lastBlinkTime = millis();
      
      Serial.print("SUCCESS: Blinking LED ");
      Serial.print(x);
      Serial.println(" times.");
    } else {
      Serial.println("ERROR: Unknown command");
    }
  } 
  else if (command == "STATUS") {
    Serial.println("--- SYSTEM STATUS ---");
    Serial.print("Pin 13 (LED) Physical State: ");
    Serial.println(digitalRead(LED_PIN) == HIGH ? "HIGH (ON)" : "LOW (OFF)");
    Serial.print("Total Blinks Executed Since Reset: ");
    Serial.println(totalBlinksExecuted);
    Serial.println("---------------------");
  } 
  else if (command == "RESET") {
    totalBlinksExecuted = 0;
    Serial.println("SUCCESS: Blink counter reset to 0.");
  } 
  else {
    // Input validation catch-all
    Serial.println("ERROR: Unknown command");
  }
}
