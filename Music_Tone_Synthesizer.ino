#include <Keypad.h>
#define BUZZER_PIN 15
#define LED_PIN 2

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int LEDC_CHANNEL = 0;
const int LEDC_RESOLUTION = 8;
const int BASE_DELAY = 100;

int currentDelay = BASE_DELAY;
String sequence[100];
int delays[100];
int seqIndex = 0;
int currentOctave = 4;

// For Replay Feature
String lastSequence[100];
int lastDelays[100];
int lastSeqLen = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  ledcSetup(LEDC_CHANNEL, 1000, LEDC_RESOLUTION);
  ledcAttachPin(BUZZER_PIN, LEDC_CHANNEL);
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    Serial.print("Key Pressed: ");
    Serial.println(key);
    blinkLED();

    if (key >= '1' && key <= '9') {
      String note = getNoteForKey(key, currentOctave);
      sequence[seqIndex] = note;
      delays[seqIndex] = currentDelay;
      seqIndex++;
      currentDelay = BASE_DELAY;
    }
    else if (key == 'A') {
      currentDelay += 100;
    }
    else if (key == '0') {
      playSequence();
      saveLastSequence();
      seqIndex = 0;
      currentDelay = BASE_DELAY;
    }
    else if (key == '*') {
      replayLastSequence();
    }
    else if (key == 'B') {
      currentOctave = 4;
    }
    else if (key == 'C') {
      currentOctave = 5;
    }
    else if (key == 'D') {
      playJingleBells();
    }
    else if (key == '#') {
      playJanaGanaMana();
    }
  }
}

String getNoteForKey(char key, int octave) {
  switch (key) {
    case '1': return "C" + String(octave);
    case '2': return "D" + String(octave);
    case '3': return "E" + String(octave);
    case '4': return "F" + String(octave);
    case '5': return "G" + String(octave);
    case '6': return "A" + String(octave);
    case '7': return "B" + String(octave);
    case '8': return "C" + String(octave + 1);
    case '9': return "D" + String(octave + 1);
    default: return "C" + String(octave);
  }
}

void playNote(String note, int delayTime) {
  float freq = getFrequency(note);
  ledcWriteTone(LEDC_CHANNEL, freq);
  delay(delayTime);
  ledcWriteTone(LEDC_CHANNEL, 0);
  delay(50);
}

void playSequence() {
  Serial.println("Playing Sequence:");
  for (int i = 0; i < seqIndex; i++) {
    Serial.print(sequence[i]);
    Serial.print(" for ");
    Serial.print(delays[i]);
    Serial.println(" ms");
    playNote(sequence[i], delays[i]);
  }
}

void saveLastSequence() {
  lastSeqLen = seqIndex;
  for (int i = 0; i < seqIndex; i++) {
    lastSequence[i] = sequence[i];
    lastDelays[i] = delays[i];
  }
}

void replayLastSequence() {
  Serial.println("Replaying Last Sequence:");
  for (int i = 0; i < lastSeqLen; i++) {
    Serial.print(lastSequence[i]);
    Serial.print(" for ");
    Serial.print(lastDelays[i]);
    Serial.println(" ms");
    playNote(lastSequence[i], lastDelays[i]);
  }
}

void playJingleBells() {
  String notes[] = {
    "E4", "E4", "E4", "E4", "E4", "E4", "E4", "G4", "C4", "D4", "E4",
    "F4", "F4", "F4", "F4", "F4", "E4", "E4", "E4", "E4", "E4", "D4", "D4", "E4", "D4", "G4"
  };
  int durations[] = {
    400, 400, 800, 400, 400, 800, 400, 400, 400, 400, 800,
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 800
  };
  Serial.println("Playing Jingle Bells:");
  for (int i = 0; i < sizeof(durations)/sizeof(durations[0]); i++) {
    playNote(notes[i], durations[i]);
  }
}

void playJanaGanaMana() {
  String notes[] = {
    "C4", "D4", "E4", "E4", "E4", "E4", "E4", "D4", "E4", "F4",
    "E4", "E4", "D4", "D4", "B3", "D4", "C4", "C4", "G3", "G3",
    "G3", "G3", "G3", "F#3", "A3", "G3", "F4", "F4", "F4", "E4",
    "E4", "F4", "D4", "F4", "E4", "E4", "E4", "D4", "D4", "D4",
    "B3", "D4", "C4", "G3", "G3", "G3", "G3", "G3", "F#3", "A3",
    "G3", "F4", "F4", "F4", "E4", "E4", "F4", "D4", "F4", "E4",
    "E4", "E4", "E4", "E4", "E4", "G4", "D4", "E4", "F4", "F4",
    "F4", "E4", "E4", "E4", "G4", "G4", "F4", "D4", "C4"
  };
  int durations[] = {
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
    400, 400, 400, 400, 400, 400, 800, 400, 400, 400,
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
    400, 400, 400, 400, 400, 400, 400, 400, 400, 400,
    400, 400, 400, 400, 400
  };
  Serial.println("Playing Jana Gana Mana:");
  for (int i = 0; i < sizeof(durations)/sizeof(durations[0]); i++) {
    playNote(notes[i], durations[i]);
  }
}

float getFrequency(String note) {
  if (note == "C4") return 261.63;
  if (note == "D4") return 293.66;
  if (note == "E4") return 329.63;
  if (note == "F4") return 349.23;
  if (note == "G4") return 392.00;
  if (note == "A4") return 440.00;
  if (note == "B4") return 493.88;
  if (note == "C5") return 523.25;
  if (note == "D5") return 587.33;
  if (note == "E5") return 659.25;
  if (note == "F5") return 698.46;
  if (note == "G5") return 783.99;
  if (note == "A5") return 880.00;
  if (note == "B5") return 987.77;
  if (note == "C6") return 1046.50;
  if (note == "D6") return 1174.66;
  if (note == "E6") return 1318.51;
  if (note == "F6") return 1396.91;
  if (note == "G6") return 1567.98;
  if (note == "G3") return 196.00;
  if (note == "F#3") return 185.00;
  if (note == "A3") return 220.00;
  if (note == "B3") return 246.94;
  return 0;
}

void blinkLED() {
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
}
