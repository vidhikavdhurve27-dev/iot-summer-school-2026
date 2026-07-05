#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Hardware Pin Assignments
const int LED_GREEN = 10;
const int LED_RED = 11;
const int PIN_BUZZER = 12;

// Password Configuration
const String CORRECT_PIN = "1234"; 
String inputPin = "";
int wrongAttempts = 0;
bool isLocked = false;
unsigned long lockStartTime = 0;

// LCD Definition (Address 0x27 is standard for I2C 16x2 screens)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 4x4 Keypad Setup Matrix
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; // Row connections
byte colPins[COLS] = {5, 4, 3, 2}; // Column connections

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void resetUI() {
  inputPin = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER PIN:");
  lcd.setCursor(0, 1);
}

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  
  resetUI();
  Serial.println("Access Control System Initialized.");
}

void loop() {
  unsigned long now = millis();

  // 1. Non-blocking System Lockout Check
  if (isLocked) {
    long elapsed = now - lockStartTime;
    long remaining = (10000 - elapsed) / 1000;

    if (remaining > 0) {
      lcd.setCursor(0, 0);
      lcd.print("SYSTEM LOCKED!  ");
      lcd.setCursor(0, 1);
      lcd.print("Wait: "); lcd.print(remaining); lcd.print("s    ");
      delay(200); // Small delay to avoid blinking text
      return;
    } else {
      isLocked = false;
      wrongAttempts = 0;
      resetUI();
      Serial.println("[SYSTEM] System automatically unlocked.");
    }
  }

  // 2. Process Keypad Inputs
  char key = customKeypad.getKey();
  
  if (key) {
    // If it's a numeric digit, collect it up to 4 digits
    if (key >= '0' && key <= '9') {
      if (inputPin.length() < 4) {
        inputPin += key;
        lcd.setCursor(inputPin.length() - 1, 1);
        lcd.print('*'); // Obfuscate password visibility
        tone(PIN_BUZZER, 1000, 50); // Audible key click
      }
    }
    // '#' acts as the Enter key
    else if (key == '#') {
      if (inputPin.length() == 4) {
        if (inputPin == CORRECT_PIN) {
          // Access Granted Process
          Serial.println("[ACCESS] Success: Correct code supplied.");
          lcd.clear();
          lcd.print("ACCESS GRANTED");
          digitalWrite(LED_GREEN, HIGH);
          tone(PIN_BUZZER, 2000, 500);
          delay(3000);
          digitalWrite(LED_GREEN, LOW);
          wrongAttempts = 0;
          resetUI();
        } 
        else {
          // Access Denied Process
          wrongAttempts++;
          Serial.print("[ACCESS] Failure: Invalid PIN code entered. Attempt Count: ");
          Serial.println(wrongAttempts);
          
          lcd.clear();
          lcd.print("ACCESS DENIED");
          digitalWrite(LED_RED, HIGH);
          
          // Sound a harsh buzzer alarm
          tone(PIN_BUZZER, 300, 800);
          delay(2000);
          digitalWrite(LED_RED, LOW);
          
          if (wrongAttempts >= 3) {
            isLocked = true;
            lockStartTime = millis();
            Serial.println("[ALERT] Too many failed attempts! Hard security freeze initialized.");
          } else {
            resetUI();
          }
        }
      }
    }
    // '*' acts as a Clear key
    else if (key == '*') {
      resetUI();
    }
  }
}
