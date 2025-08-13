/*
Nano ESP32 Neopixel - Create a mini moving rainbow.
Demonstrates:
  - use of the Adafruit NeoPixel.h library
  - substitution of a look-up table for a complex calculation
Enhancement ideas:
  - add a gamma correction look-up table
  - add light fades, random light values, light patterns based on
    analog or digital inputs
  - make a lightsabre, reaction timer game, or audio response system
*/

#include <Adafruit_NeoPixel.h>
const int NEO_PIN = 5;      // Pin D2 - NeoPixel.h uses native ESP32 pin numbers
int numPixels = 8;
int rainbowStep = int(180 / numPixels);
Adafruit_NeoPixel strip(numPixels, NEO_PIN, NEO_GRB + NEO_KHZ800);

// Program variables
int pixelDelay = 10;        // Delay between NeoPixel updates
int rainbow = 0;            // Rainbow index into sine table

// Small sine table for easy rainbow lookups
const uint8_t sine[180] = {
0, 0, 1, 2, 3, 4, 6, 9,
11, 14, 17, 21, 25, 29, 33, 37,
42, 47, 53, 58, 64, 70, 76, 82,
88, 95, 101, 108, 115, 121, 128, 134,
140, 147, 154, 160, 167, 173, 179, 185,
191, 197, 202, 208, 213, 218, 222, 226,
231, 234, 238, 241, 244, 246, 249, 251,
252, 253, 254, 255, 255, 255, 254, 253,
252, 251, 249, 246, 244, 241, 238, 234,
231, 226, 222, 218, 213, 208, 202, 197,
191, 185, 179, 173, 167, 160, 154, 147,
140, 134, 127, 121, 115, 108, 101, 95,
88, 82, 76, 70, 64, 58, 53, 47,
42, 37, 33, 29, 25, 21, 17, 14,
11, 9, 6, 4, 3, 2, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0};

void setup() {
  strip.begin();       // Initialize pixels to off
  strip.show();        // Write data to all pixels
}

void loop() {
  strip.clear();

  for(int i = 0; i < numPixels; i++) {
    strip.setPixelColor(i, strip.Color(sine[(rainbow + i * rainbowStep) % 180],
      sine[(rainbow + 60 + i * rainbowStep) % 180],
      sine[(rainbow + 120 + i * rainbowStep) % 180]));
    strip.show();
  }
  rainbow ++;
  if(rainbow > 180) {
    rainbow = 0;
  }
  delay(pixelDelay);
}