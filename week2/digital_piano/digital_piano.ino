// Pin Definitions
const int BTN_DO = 2;
const int BTN_RE = 3;
const int BTN_MI = 4;
const int BTN_FA = 5;
const int BUZZER_PIN = 9;

// Note Frequencies (Hz)
const int NOTE_DO = 262;
const int NOTE_RE = 294;
const int NOTE_MI = 330;
const int NOTE_FA = 349;

void setup() {
  pinMode(BTN_DO, INPUT_PULLUP);
  pinMode(BTN_RE, INPUT_PULLUP);
  pinMode(BTN_MI, INPUT_PULLUP);
  pinMode(BTN_FA, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Read button states (LOW means pressed because of INPUT_PULLUP)
  bool pressDo = (digitalRead(BTN_DO) == LOW);
  bool pressRe = (digitalRead(BTN_RE) == LOW);
  bool pressMi = (digitalRead(BTN_MI) == LOW);
  bool pressFa = (digitalRead(BTN_FA) == LOW);

  // Play highest priority single note, otherwise silence
  if (pressDo) {
    tone(BUZZER_PIN, NOTE_DO);
  } else if (pressRe) {
    tone(BUZZER_PIN, NOTE_RE);
  } else if (pressMi) {
    tone(BUZZER_PIN, NOTE_MI);
  } else if (pressFa) {
    tone(BUZZER_PIN, NOTE_FA);
  } else {
    noTone(BUZZER_PIN);
  }
}
