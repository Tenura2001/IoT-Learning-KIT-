#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==== OLED ====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ==== Pin Definitions ====
const int LED_PIN = 25;       // DAC pin
const int BUZZER_PIN = 2;

const int POT_PIN = 35;
const int BTN1_PIN = 36;
const int BTN2_PIN = 39;
const int BTN3_PIN = 34;

// Shift Register Pins (74HC595)
const int LATCH_PIN = 4;
const int CLOCK_PIN = 13;
const int DATA_PIN = 15;

// ==== Variables ====
int knightPos = 0;
int knightDir = 1;  // 1 = right, -1 = left
unsigned long lastKnightUpdate = 0;
const int knightDelay = 100;  // ms

void setup() {
  Serial.begin(115200);

  // Buttons
  pinMode(BTN1_PIN, INPUT);
  pinMode(BTN2_PIN, INPUT);
  pinMode(BTN3_PIN, INPUT);

  // LED, Buzzer, Shift Reg
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);

  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found!"));
    while (true);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  // === Potentiometer controls LED ===
  int potValue = analogRead(POT_PIN); // 0–4095
  int brightness = map(potValue, 0, 4095, 0, 255);
  dacWrite(LED_PIN, brightness);

  // === Buttons ===
  bool btn1 = digitalRead(BTN1_PIN) == LOW;
  bool btn2 = digitalRead(BTN2_PIN) == LOW;
  bool btn3 = digitalRead(BTN3_PIN) == LOW;

  // === Buzzer tones ===
  if (btn1) tone(BUZZER_PIN, 440);  // A4
  else if (btn2) tone(BUZZER_PIN, 880); // A5
  else if (btn3) tone(BUZZER_PIN, 1760); // A6
  else noTone(BUZZER_PIN);

  // === OLED Display ===
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Pot: "); display.println(potValue);
  display.print("Brightness: "); display.println(brightness);
  display.print("BTN1: "); display.println(btn1 ? "ON" : "OFF");
  display.print("BTN2: "); display.println(btn2 ? "ON" : "OFF");
  display.print("BTN3: "); display.println(btn3 ? "ON" : "OFF");
  display.display();

  // === Knight Rider on Shift Register ===
  if (millis() - lastKnightUpdate > knightDelay) {
    byte pattern = 1 << knightPos;
    updateShiftRegister(pattern);
    knightPos += knightDir;
    if (knightPos == 7 || knightPos == 0) knightDir *= -1;
    lastKnightUpdate = millis();
  }

  delay(10); // Small delay
}

void updateShiftRegister(byte data) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data);
  digitalWrite(LATCH_PIN, HIGH);
}
