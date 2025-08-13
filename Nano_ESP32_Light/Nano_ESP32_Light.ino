/*
Nano ESP32 Light - Demonstrates phototransistor input
Demonstrates:
  - analog input 
  - serial output
Enhancement ideas:
  - Light level monitoring
  - Nightlight
*/

// Define hardware devices
const int Q1 = A7;

// Define software variables
int lightLevel;

// Pre-defined Arduino Nano ESP32 LEDS (listed here for name reference)
// LED_BUILTIN (D13)          // Yellow LED
// LED_BLUE                   // RGB LED blue element (active LOW)
// LED_GREEN                  // RGB LED green element (active LOW)
// LED_RED                    // RGB LED red element(active LOW)

void setup() {
  pinMode(Q1, INPUT);         // Using an external pull-up resistor
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the USB serial port to start up.
  }
}

void loop() {
  lightLevel = analogRead(Q1);

  if (lightLevel > 3000) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.print("Light level: ");
  Serial.println(lightLevel);
  delay(100);
}
