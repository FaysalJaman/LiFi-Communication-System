#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int ldrPin = A0;
int threshold = 170;
int bitDuration = 300;

// -------- Stable LDR --------
int readLDR() {
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(ldrPin);
    delay(2);
  }
  return sum / 5;
}

// -------- WAIT FOR IDLE (LOW) --------
void waitForIdle() {
  while (readLDR() > threshold); // wait until no light
}

// -------- WAIT FOR START --------
bool detectStart() {
  if (readLDR() > threshold + 20) {
    delay(bitDuration / 2);

    if (readLDR() > threshold) {
      return true;
    }
  }
  return false;
}

// -------- RECEIVE BYTE --------
byte receiveByte() {
  byte value = 0;

  for (int i = 7; i >= 0; i--) {
    int bitVal = (readLDR() > threshold) ? 1 : 0;
    bitWrite(value, i, bitVal);
    delay(bitDuration);
  }

  return value;
}

// -------- SETUP --------
void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("LiFi Ready");
  delay(2000);
  lcd.clear();
}

// -------- LOOP --------
void loop() {

  // Step 1: ensure line is idle (no transmission)
  waitForIdle();

  // Step 2: wait for real start
  if (detectStart()) {

    // Move to first data bit center
    delay(bitDuration);

    byte received = receiveByte();
    char data = (char)received;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Received:");
    lcd.setCursor(0,1);
    lcd.print(data);

    delay(500);
  }
}
