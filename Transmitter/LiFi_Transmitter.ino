#include <Keypad.h>

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Pins (as you defined)
byte rowPins[ROWS] = {A5, A4, A3, A2};
byte colPins[COLS] = {A1, A0, 12, 11};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int ledPin = 8;

// Timing (VERY IMPORTANT for sync)
int bitDuration = 100; // ms

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

// Convert char → 8-bit binary transmission
void sendData(char data) {
  byte value = (byte)data;

  Serial.print("Sending: ");
  Serial.print(data);
  Serial.print(" Binary: ");

  // Send 8 bits (MSB first)
  for (int i = 7; i >= 0; i--) {
    int bitVal = bitRead(value, i);
    
    Serial.print(bitVal);

    digitalWrite(ledPin, bitVal);
    delay(bitDuration);
  }

  Serial.println();

  // Small gap between characters
  digitalWrite(ledPin, LOW);
  delay(300);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    sendData(key);
  }
}
