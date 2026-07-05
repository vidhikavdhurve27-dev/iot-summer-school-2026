/**
 * @file vending_machine.ino
 * @brief Finite State Machine for an automated vending unit layout.
 * 
 * STATE TRANSITION DIAGRAM:
 * 
 *       +---------[ Cancel Button ]----------+
 *       |                                    |
 *       v                                    |
 *   [ IDLE ] --- (Insert Coin) ---> [ COIN_INSERTED ]
 *      ^                                     |
 *      |                               (Select Item)
 *      |                                     v
 *   [ DISPENSING ] <--- (Auto 2s) --- [ ITEM_SELECTED ]
 * 
 *  *Note: Pressing 'Cancel' at COIN_INSERTED or ITEM_SELECTED resets back to IDLE.
 */

// ==========================================
// 1. DATA TYPES & PROTOTYPES (Must be first for Tinkercad)
// ==========================================
enum VendingState { 
  IDLE, 
  COIN_INSERTED, 
  ITEM_SELECTED, 
  DISPENSING 
};

// Forward declaration tells loop() that this function exists later
void changeState(VendingState newState);
void updateHardwareIndicators();

// ==========================================
// 2. PIN DEFINITIONS & VARIABLES
// ==========================================
const int BTN_COIN        = 2; 
const int BTN_ITEM_SELECT = 3; 
const int BTN_CANCEL      = 4; 

const int LED_RED    = 11; 
const int LED_YELLOW = 12; 
const int LED_GREEN  = 13; 

VendingState currentState = IDLE;

unsigned long stateStartTime = 0;
const unsigned long DELAY_SELECT = 1500; 
const unsigned long DELAY_DISPENSE = 3000; 

bool lastCoinState   = HIGH;
bool lastSelectState = HIGH;
bool lastCancelState = HIGH;

// ==========================================
// 3. MAIN ARDUINO FUNCTIONS
// ==========================================
void setup() {
  Serial.begin(9600);
  
  pinMode(BTN_COIN, INPUT_PULLUP);
  pinMode(BTN_ITEM_SELECT, INPUT_PULLUP); 
  pinMode(BTN_CANCEL, INPUT_PULLUP);
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  updateHardwareIndicators();
  Serial.println("[SYSTEM BOOT]: State shifted to -> IDLE. Awaiting coin insertion.");
  stateStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  
  bool currentCoinState   = digitalRead(BTN_COIN);
  bool currentSelectState = digitalRead(BTN_ITEM_SELECT); 
  bool currentCancelState = digitalRead(BTN_CANCEL);

  if (currentCancelState == LOW && lastCancelState == HIGH) {
    if (currentState != IDLE) {
      Serial.println("[TRANSITION]: Action -> CANCEL. Refunding assets.");
      changeState(IDLE);
    }
  }

  switch (currentState) {
    case IDLE:
      if (currentCoinState == LOW && lastCoinState == HIGH) {
        Serial.println("[TRANSITION]: Action -> COIN INSERTED. Balance updated.");
        changeState(COIN_INSERTED);
      }
      break;

    case COIN_INSERTED:
      if (currentSelectState == LOW && lastSelectState == HIGH) {
        Serial.println("[TRANSITION]: Action -> ITEM SELECTED. Checking inventory levels.");
        changeState(ITEM_SELECTED);
      }
      break;

    case ITEM_SELECTED:
      if (currentTime - stateStartTime >= DELAY_SELECT) {
        Serial.println("[TRANSITION]: Action -> PROCESSING. Initializing conveyor dropping mechanics.");
        changeState(DISPENSING);
      }
      break;

    case DISPENSING:
      if (currentTime - stateStartTime >= DELAY_DISPENSE) {
        Serial.println("[TRANSITION]: Action -> DISPENSE COMPLETE. Enjoy your item!");
        changeState(IDLE);
      }
      break;
  }

  lastCoinState   = currentCoinState;
  lastSelectState = currentSelectState;
  lastCancelState = currentCancelState;
}

// ==========================================
// 4. FUNCTION IMPLEMENTATIONS
// ==========================================
void changeState(VendingState newState) {
  currentState = newState;
  stateStartTime = millis(); 
  updateHardwareIndicators();
}

void updateHardwareIndicators() {
  switch (currentState) {
    case IDLE: 
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);
      break;
    case COIN_INSERTED: 
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      break;
    case ITEM_SELECTED: 
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, HIGH);
      break;
    case DISPENSING: 
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, HIGH);
      break;
  }
}

