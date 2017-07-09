#include "FastLED.h"
#include <EEPROM.h>
// How many leds in your strip?
#define NUM_LEDS 88

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13


#define LED_TYPE    WS2812 // Any LEDS's that FastLED supports.
#define COLOR_ORDER GRB // LED Order (If your colors are wrong, flip this around.)

// Define the array of leds
CRGB leds[NUM_LEDS];
String incomingString = String("");   // for incoming serial data
String MODE = String("solidrainbow");
int COLOR[] = {10, 100, 255};
// Do some setup.
void setup() {
  Serial.begin(57600); // reduce for slower arduinos.
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(50);
  Serial.println("Ready");
}

void setSetting(String input) {
  MODE = input;
}

void readSerial() {

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingString = Serial.readString();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingString);
    setSetting(incomingString);
  }
}

void solidColor() {
  // Set all LED's the same color.
  for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB(COLOR[0], COLOR[1], COLOR[2]);
  }

  FastLED.show();
  delay(200);
}

void solidRainbow() {
  static uint8_t hue = 0;
	// For each LED
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to hue
		leds[i] = CHSV(hue, 255, 255);
	}
  // Show the leds
  FastLED.show();
  // Change hue
  hue++;
  // Wait a little bit before we loop around and do it again
  delay(20);
}

void fadeall() { // Fades LED's over time.
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}

void circleColor() {
  for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to COLOR
		leds[i] = CRGB(COLOR[0], COLOR[1], COLOR[2]);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
}

void circleRainbow() {
  static uint8_t hue = 0;
	// First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
}

void loop() {
  readSerial();

  if (MODE.equals("solid")) {
    solidColor();
  }

  if (MODE.equals("solidrainbow")) {
    solidRainbow();
  }

  if (MODE.equals("circle")) {
    circleColor();
  }

  if(MODE.equals("rainbow")) {
    circleRainbow();
  }

  Serial.print("current mode is: ");
  Serial.println(MODE);

  // Serial.print("x");
}
