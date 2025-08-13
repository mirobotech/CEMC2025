/*
Nano ESP32 Potentiometer - Demonstrates potentiometer input
Demonstrates:
  - analog input and map() function conversion
  - analog and serial output
Enhancement ideas:
  - colour mixer
  - motor speed control
*/

// Define hardware devices
const int POT1 = A0;

// Define software variables
int potPosition;
int brightness;

void setup() {
  pinMode(POT1, INPUT);
  pinMode(LED_RED, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the USB serial port to start up.
  }
}

void loop() {
  potPosition = analogRead(POT1);
  brightness = map(potPosition, 0, 4095, 255, 0);
  analogWrite(LED_RED, brightness);

  Serial.print("Pot position: ");
  Serial.print(potPosition);
  Serial.print(" \tBrightness: ");
  Serial.println(255 - brightness);

  delay(100);
}
