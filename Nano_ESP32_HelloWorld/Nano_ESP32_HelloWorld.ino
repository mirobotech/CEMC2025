/*
Nano ESP32 HelloWorld - Blinks Arduino Nano ESP32 blinkable LEDs
Demonstrates:
  - basic program structure
  - control of the built-in LEDs
Enhancement ideas:
  - Binary exploration of 3-bit color combinations
  - Simulated traffic light
*/

// Pre-defined Arduino Nano ESP32 LEDS (listed here for name reference)
// LED_BUILTIN (D13)          // Yellow LED
// LED_BLUE                   // RGB LED blue element (active LOW)
// LED_GREEN                  // RGB LED green element (active LOW)
// LED_RED                    // RGB LED red element(active LOW)

void setup() {
  // Setup code runs once:
  pinMode(LED_BUILTIN, OUTPUT); // Active HIGH
  pinMode(LED_BLUE, OUTPUT);    // Active LOW
  pinMode(LED_GREEN, OUTPUT);   // Active LOW
  pinMode(LED_RED, OUTPUT);     // Active LOW
}

void loop() {
  // Main loop code runs repeatedly:
  digitalWrite(LED_BLUE, HIGH);   // Blue LED off
  digitalWrite(LED_BUILTIN, HIGH);  // LED on
  delay(500);

  digitalWrite(LED_BUILTIN, LOW); // LED off
  digitalWrite(LED_GREEN, LOW);   // Green LED on
  delay(500);
  
  digitalWrite(LED_GREEN, HIGH);  // Green LED off
  digitalWrite(LED_RED, LOW);     // Red LED on
  delay(500);
  
  digitalWrite(LED_RED, HIGH);    // Red LED off
  digitalWrite(LED_BLUE, LOW);    // Blue LED on
  delay(500);
}
