#define BUZZER 2  // Buzzer connected to pin 2

// Notes frequencies (Hz)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_Bb  466
#define NOTE_C5  523

// Happy Birthday melody
int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_Bb, NOTE_Bb, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

// Durations (smaller = faster, larger = slower)
int noteDurations[] = {
  4, 8, 4, 4, 4, 2,   // "Happy Birthday to you"
  4, 8, 4, 4, 4, 2,   // "Happy Birthday to you"
  4, 8, 4, 4, 4, 4, 2, // "Happy Birthday dear [Name]"
  4, 8, 4, 4, 4, 2    // "Happy Birthday to you"
};

void setup() {
  int notes = sizeof(melody) / sizeof(int);

  for (int i = 0; i < notes; i++) {
    int duration = 1000 / noteDurations[i];   // base duration
    tone(BUZZER, melody[i], duration);        // play note
    delay(duration * 1.5);                    // add gap for better rhythm
    noTone(BUZZER);                           // stop tone between notes

    // extra pause at phrase ends
    if (i == 5 || i == 11 || i == 18) {
      delay(500);
    }
  }
}

void loop() {
  // Play only once
}
