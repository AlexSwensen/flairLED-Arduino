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

// Setup some settings we will use elsewhere
String incomingString = String("");   // for incoming serial data

// setup data type for color
struct color_t {
  int red;
  int green;
  int blue;
};

// setup settings structure
struct setting_t {
  color_t color;
  String mode;
} settings;

// Do some setup.
void setup() {
  Serial.setTimeout(250); // Set Serial timeout (increase for slower Arduino's)
  Serial.begin(19200); // Set Baud Rate (reduce for slower Arduino's.)

  // setup defaults for settings.
  settings.color = {10, 100, 255};
  settings.mode = "solidrainbow";

  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(50);
  Serial.println("Ready");
}

/**
* Parses serial input and sets modes to appropriate things.
*/
void parseSerial(String input) {

  if (input.indexOf('mode')) {
    input.trim(); // removes whitespace
    input.remove(0, 5); // removes 'mode'
    settings.mode = input;
  }

  if (input.indexOf('color')) {
    input.trim(); //removes whitespace
    input.remove(0, 6);
  }
}

void readSerial() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingString = Serial.readString();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingString);
    parseSerial(incomingString);
  }
}

void solidColor() {
  // Set all LED's the same color.
  for (int i = 0; i < NUM_LEDS; i++) {
          leds[i] = CRGB(settings.color.red, settings.color.blue, settings.color.green);
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
		leds[i] = CRGB(settings.color.red, settings.color.blue, settings.color.green);
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

  if (settings.mode.equals("solid")) {
    solidColor();
  }

  if (settings.mode.equals("solidrainbow")) {
    solidRainbow();
  }

  if (settings.mode.equals("circle")) {
    circleColor();
  }

  if(settings.mode.equals("rainbow")) {
    circleRainbow();
  }

  Serial.print("current mode is: ");
  Serial.println(settings.mode);

  // Serial.print("x");
}
