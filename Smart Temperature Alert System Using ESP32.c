#include <DHT.h>

// ==========================
// DHT11
// ==========================
#define DHTPIN 16
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// ==========================
// RGB LED 1
// ==========================
#define R1 13
#define G1 14
#define B1 27

// ==========================
// RGB LED 2
// ==========================
#define R2 26
#define G2 32
#define B2 33

// ==========================
// BUZZER
// ==========================
#define BUZZER 25


// =====================================================
// HAPPY BIRTHDAY TUNE
// =====================================================

// Notes
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784

int melody[] = {

  NOTE_C4, NOTE_C4,
  NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,

  NOTE_C4, NOTE_C4,
  NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,

  NOTE_C4, NOTE_C4,
  NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,

  NOTE_AS4, NOTE_AS4,
  NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};


// Note durations
int durations[] = {

  250, 250,
  500, 500, 500, 1000,

  250, 250,
  500, 500, 500, 1000,

  250, 250,
  500, 500, 500, 500, 1000,

  250, 250,
  500, 500, 500, 1000
};


// =====================================================
// SET BOTH RGB LEDs
// =====================================================

void setColor(int r, int g, int b) {

  analogWrite(R1, r);
  analogWrite(G1, g);
  analogWrite(B1, b);

  analogWrite(R2, r);
  analogWrite(G2, g);
  analogWrite(B2, b);
}


// =====================================================
// PLAY HAPPY BIRTHDAY
// =====================================================

void playHappyBirthday() {

  int numberOfNotes = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < numberOfNotes; i++) {

    tone(BUZZER, melody[i]);

    delay(durations[i]);

    noTone(BUZZER);

    delay(40);
  }
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // DHT
  dht.begin();

  // RGB LED 1
  pinMode(R1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(B1, OUTPUT);

  // RGB LED 2
  pinMode(R2, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(B2, OUTPUT);

  // Buzzer
  pinMode(BUZZER, OUTPUT);

  // Initial state
  setColor(0, 0, 0);
  noTone(BUZZER);

  Serial.println("================================");
  Serial.println(" Temperature Alert System");
  Serial.println("================================");
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  float temperature = dht.readTemperature();


  // ===================================================
  // DHT11 ERROR
  // ===================================================

  if (isnan(temperature)) {

    Serial.println("DHT11 reading failed!");

    // PURPLE = SENSOR ERROR
    setColor(255, 0, 255);

    tone(BUZZER, 1500);
    delay(100);

    noTone(BUZZER);

    delay(500);

    return;
  }


  // ===================================================
  // PRINT TEMPERATURE
  // ===================================================

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");


  // ===================================================
  // <= 25°C
  // NORMAL
  // ===================================================

  if (temperature <= 25) {

    Serial.println("STATUS: NORMAL");

    // GREEN
    setColor(0, 255, 0);

    // BUZZER OFF
    noTone(BUZZER);

    delay(1000);
  }


  // ===================================================
  // 26°C - 30°C
  // WARNING
  // ===================================================

  else if (temperature <= 30) {

    Serial.println("STATUS: WARNING");

    // YELLOW
    setColor(255, 180, 0);

    // Short warning beep
    tone(BUZZER, 1000);
    delay(100);

    noTone(BUZZER);

    delay(900);
  }


  // ===================================================
  // > 30°C
  // HIGH TEMPERATURE
  // ===================================================

  else {

    Serial.println("STATUS: HIGH TEMPERATURE!");

    // RED
    setColor(255, 0, 0);

    // Play Happy Birthday tune
    playHappyBirthday();

    noTone(BUZZER);

    delay(500);
  }
}
