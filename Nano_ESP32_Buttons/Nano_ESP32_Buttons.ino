/*
Nano ESP32 Buttons - Demonstrates pushbutton input
Demonstrates:
  - digital input 
  - analog output
Enhancement ideas:
  - RGB colour mixer
  - motor speed control
*/

// Define hardware devices
const int PB1 = D0;

// Define software variables
bool PB1State;
int green = 0;                // Starting green value
bool greenIncr = true;        // Increasing green values

// Pre-defined Arduino Nano ESP32 LEDS (listed here for name reference)
// LED_BUILTIN (D13)          // Yellow LED
// LED_BLUE                   // RGB LED blue element (active LOW)
// LED_GREEN                  // RGB LED green element (active LOW)
// LED_RED                    // RGB LED red element(active LOW)

void setup() {
  pinMode(PB1, INPUT_PULLUP);   // Use internal pull-up resistor
  pinMode(LED_GREEN, OUTPUT);   // Active LOW
}

void loop() {
  PB1State = digitalRead(PB1);

  if(PB1State == LOW && greenIncr == true) {
    if(green < 255) {
      green++;
      if(green == 255) {
        greenIncr = false;
      }
    }
  }

  if(PB1State == LOW && !greenIncr) {
    if(green > 0) {
      green--;
      if(green == 0) {
        greenIncr = true;
      }
    }
  }

  analogWrite(LED_GREEN, 255 - green);

  delay(10);
}
