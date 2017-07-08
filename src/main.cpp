#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 67

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
        Serial.begin(57600);
        Serial.println("resetting");
        LEDS.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
        LEDS.setBrightness(50);
}

void solidColor() {
        // First slide the led in one direction
        for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CRGB(20, 20, 20);
        }

        FastLED.show();
        // Serial.print(COLORS[x]);
        // Serial.print('\n');
        delay(200);
}
void loop() {
        solidColor();
        Serial.print("x");
}
