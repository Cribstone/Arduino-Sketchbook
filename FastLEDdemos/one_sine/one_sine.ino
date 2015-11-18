
/* one_sine

By: Andrew Tuline

Date: Jan, 2015

The Cylon routine included with the FastLED examples has code to move an LED in one direction, and more code to move it in another. Kind of boring, but it works.

This routine DOES only go in one directon a a time, but it's got a LOT of variables you can tweak.

*/


#define qsubd(x, b)  ((x>b)?wavebright:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0

#include "FastLED.h"                                          // FastLED library. Preferably the latest copy of FastLED 2.1.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 12                                             // Serial data pin for WS2812B or WS2801.
#define LED_CK 11                                             // Serial clock pin for WS2801 or APA102.
#define COLOR_ORDER BGR                                       // It's GRB for WS2812B
#define LED_TYPE APA102                                       // What kind of strip are you using?
#define NUM_LEDS 20                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

uint8_t max_bright = 64;                                     // Overall brightness definition. It can be changed on the fly.


// Initialize changeable global variables. Play around with these!!!
uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int loopdelay = 10;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 10;                                        // Brightness of background colour



void setup() {
  Serial.begin(57600);
  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);                // FastLED 2.1 Power management set at 5V, 500mA
} // setup()


void loop () {
  one_sine();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(loopdelay);
} // loop()


void one_sine() {                                                              // This is the heart of this program. Sure is short.
  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbright);                                     // First set a background colour, but fully saturated.
    leds[k] += CHSV(thishue, allsat, thisbright);                             // Then assign a hue to any that are bright enough.
  }
  bgclr++;                                                                    // You can change the background colour or remove this and leave it fixed.
} // one_sine()
