/**
 * digital_piano.ino
 */

const int PIN_BUZZER = 3;
const int BTN_PINS[4] = {4, 5, 6, 8}; // Key buttons 1 to 4
const int PIN_TOGGLE = 9;             // 5th Button for Scale Mode

// Tone mappings: Index 0-3 are Do, Re, Mi, Fa. Index 4 is Sol.
int majorNotes[5] = {262, 294, 330, 349, 392}; // Standard Major
int minorNotes[5] = {262, 294, 311, 349, 392}; // Minor Scale (Flat 3rd: Mi drops to 311Hz)

void setup() {
  pinMode(PIN_BUZZER, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(BTN_PINS[i], INPUT_PULLUP);
  }
  pinMode(PIN_TOGGLE, INPUT_PULLUP);
}

void loop() {
  bool btnState[4];
  int activeCount = 0;
  
  // Read inputs (LOW means active/pressed due to INPUT_PULLUP)
  for (int i = 0; i < 4; i++) {
    btnState[i] = (digitalRead(BTN_PINS[i]) == LOW);
    if (btnState[i]) activeCount++;
  }
  
  bool isMinor = (digitalRead(PIN_TOGGLE) == LOW);
  int* currentScale = isMinor ? minorNotes : majorNotes;

  // Rule 1: Multi-press chord substitute
  if (activeCount >= 2) {
    tone(PIN_BUZZER, currentScale[4]); // Play Sol (392Hz)
  } 
  // Rule 2: Single note playback
  else if (activeCount == 1) {
    for (int i = 0; i < 4; i++) {
      if (btnState[i]) {
        tone(PIN_BUZZER, currentScale[i]);
        break;
      }
    }
  } 
  // Rule 3: Mute on release
  else {
    noTone(PIN_BUZZER);
  }
}
