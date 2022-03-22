#include <FastLED.h>
#define FastLED_INTERNAL

// How many leds in your strip?
#define NUM_LEDS 20

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN D2


// Define the array of leds
CRGB leds[NUM_LEDS];

void sweep(CRGB color);

void setup() { 
  Serial.begin(115200);
  noInterrupts();
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setBrightness(10);

}

void loop() {
  sweep(CRGB::Red, 100);
}

void sweep(CRGB color, int speed) {
  static float pos = 0;
  if (pos >= 5) {
    pos = 0;
  }
  FastLED.clear(false);

  for (int i = 0; i < 5; i++) {
    leds[(pos+(i*5))] = color;
  }
  FastLED.show();
  pos++;
  delay(speed);
}
