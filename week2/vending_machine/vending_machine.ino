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

enum VendingState { IDLE, COIN_INSERTED, ITEM_SELECTED, DISPENSING };
VendingState currentState = IDLE;

const int BTN_COIN = 4;
const int BTN_SELECT = 5;
const int BTN_CANCEL = 6;

const int LED_COIN = 10;     // Glows when a coin is stored
const int LED_SELECT = 11;   // Glows when item choice is locked
const int LED_DISPENSE = 12; // Glows while releasing product

unsigned long dispenseStartTime = 0;

void transitionTo(VendingState newState, String context) {
  currentState = newState;
  Serial.print("[Transition] Event: "); Serial.print(context);
  Serial.print(" -> State: ");
  
  // Reset all LEDs first
  digitalWrite(LED_COIN, LOW);
  digitalWrite(LED_SELECT, LOW);
  digitalWrite(LED_DISPENSE, LOW);

  switch (currentState) {
    case IDLE:
      Serial.println("IDLE (Awaiting transaction...)");
      break;
    case COIN_INSERTED:
      Serial.println("COIN_INSERTED (Credits verified)");
      digitalWrite(LED_COIN, HIGH);
      break;
    case ITEM_SELECTED:
      Serial.println("ITEM_SELECTED (Inventory choice approved)");
      digitalWrite(LED_SELECT, HIGH);
      break;
    case DISPENSING:
      Serial.println("DISPENSING (Dropping component item)");
      digitalWrite(LED_DISPENSE, HIGH);
      dispenseStartTime = millis();
      break;
  }
}

void setup() {
  pinMode(BTN_COIN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_CANCEL, INPUT_PULLUP);
  
  pinMode(LED_COIN, OUTPUT);
  pinMode(LED_SELECT, OUTPUT);
  pinMode(LED_DISPENSE, OUTPUT);
  
  Serial.begin(9600);
  transitionTo(IDLE, "System Power On");
}

void loop() {
  // Read inputs (LOW indicates button is physically pressed down)
  bool coinPressed = (digitalRead(BTN_COIN) == LOW);
  bool selectPressed = (digitalRead(BTN_SELECT) == LOW);
  bool cancelPressed = (digitalRead(BTN_CANCEL) == LOW);

  // Global Cancel Rule: Active in intermediate states
  if (cancelPressed && (currentState == COIN_INSERTED || currentState == ITEM_SELECTED)) {
    transitionTo(IDLE, "User pressed CANCEL (Refunding)");
    delay(300); // Simple mechanical button debounce
    return;
  }

  // State Handler Machine Matrix
  switch (currentState) {
    case IDLE:
      if (coinPressed) {
        transitionTo(COIN_INSERTED, "COIN_INSERT BUTTON");
        delay(300);
      }
      break;

    case COIN_INSERTED:
      if (selectPressed) {
        transitionTo(ITEM_SELECTED, "SELECT_ITEM BUTTON");
        delay(300);
      }
      break;

    case ITEM_SELECTED:
      // Automatic drop trigger pipeline step
      transitionTo(DISPENSING, "Auto-processing choice");
      break;

    case DISPENSING:
      // Non-blocking timer tracking release loop window (lasts 2000ms)
      if (millis() - dispenseStartTime >= 2000) {
        transitionTo(IDLE, "Dispense Cycle Completed Successfully");
      }
      break;
  }
}
