// Pin Definitions
const int BTN_DO = 2;
const int BTN_RE = 3;
const int BTN_MI = 4;
const int BTN_FA = 5;
const int BTN_TOGGLE = 6; // 5th button for Major/Minor mode
const int BUZZER_PIN = 9;

// Note Frequencies for Major Scale (Hz)
const int MAJ_DO  = 262; // C4
const int MAJ_RE  = 294; // D4
const int MAJ_MI  = 330; // E4
const int MAJ_FA  = 349; // F4
const int MAJ_SOL = 392; // G4

// Note Frequencies for Minor Scale (Hz)
const int MIN_DO  = 262; // C4
const int MIN_RE  = 277; // C#4 / Db4 (Flat 2nd alternate)
const int MIN_MI  = 311; // D#4 / Eb4 (Flat 3rd)
const int MIN_FA  = 349; // F4
const int MIN_SOL = 392; // G4

// State variables for mode toggle
bool isMajorMode = true;
bool lastToggleState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50; 

void setup() {
  pinMode(BTN_DO, INPUT_PULLUP);
  pinMode(BTN_RE, INPUT_PULLUP);
  pinMode(BTN_MI, INPUT_PULLUP);
  pinMode(BTN_FA, INPUT_PULLUP);
  pinMode(BTN_TOGGLE, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // --- 1. Direct Toggle Switch Mode Selection ---
  // Switch flipped to Right (GND) = LOW -> Minor Mode
  // Switch flipped to Left (Open) = HIGH -> Major Mode
  if (digitalRead(BTN_TOGGLE) == LOW) {
    isMajorMode = false; // Minor scale active
  } else {
    isMajorMode = true;  // Major scale active
  }

  // --- 2. Tone Matrix Logic ---
  bool pressDo = (digitalRead(BTN_DO) == LOW);
  bool pressRe = (digitalRead(BTN_RE) == LOW);
  bool pressMi = (digitalRead(BTN_MI) == LOW);
  bool pressFa = (digitalRead(BTN_FA) == LOW);

  int pressedCount = pressDo + pressRe + pressMi + pressFa;

  if (pressedCount >= 2) {
    // Shared chord fallback note
    tone(BUZZER_PIN, isMajorMode ? MAJ_SOL : MIN_SOL);
  } else if (pressedCount == 1) {
    if (pressDo)       tone(BUZZER_PIN, isMajorMode ? MAJ_DO : MIN_DO);
    else if (pressRe)  tone(BUZZER_PIN, isMajorMode ? MAJ_RE : MIN_RE);
    else if (pressMi)  tone(BUZZER_PIN, isMajorMode ? MAJ_MI : MIN_MI);
    else if (pressFa)  tone(BUZZER_PIN, isMajorMode ? MAJ_FA : MIN_FA);
  } else {
    noTone(BUZZER_PIN);
  }
}
