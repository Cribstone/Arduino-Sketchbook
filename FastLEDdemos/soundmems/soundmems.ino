
/* soundmems

By: Andrew Tuline

Date: Oct, 2014

Basic code to read from the Sparkfun INMP401 microphone and display CHSV output. You may need to only use battery power as noise from the USB can affect the signals.

*/


#include "FastLED.h"                                          // FastLED library.
 
// Fixed definitions cannot change on the fly.
#define LED_DT 6                                             // Data pin to connect to the strip.
//#define LED_CK 11
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B
#define LED_TYPE WS2811                                       // What kind of strip are you using (WS2801, WS2812B or APA102)?
#define NUM_LEDS 5                                          // Number of LED's.

// Initialize changeable global variables.
uint8_t max_bright = 128;                                     // Overall brightness definition. It can be changed on the fly.

struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

#define MIC_PIN 5                                             // Analog port for microphone
#define DC_OFFSET  32                                         // DC offset in mic signal - if unusure, leave 0
 

// Initialize global variables for sequences
int thisdelay = 4;                                            // A delay value for the sequence(s)


void setup() {
  // This is only needed on 5V Arduinos (Uno, Leonardo, etc.).
  // Connect 3.3V to mic AND TO AREF ON ARDUINO and enable this
  // line.  Audio samples are 'cleaner' at 3.3V.
  // COMMENT OUT THIS LINE FOR 3.3V ARDUINOS (FLORA, ETC.):
  analogReference(EXTERNAL);
  
  Serial.begin(57600);

  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);          // Use this for WS2812B
  //LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 500);
} // setup()


void loop() {
  soundmems();
  show_at_max_brightness_for_power();
  delay_at_max_brightness_for_power(thisdelay*2.5);
} // loop()


void soundmems() {
  int n;
  for (int i = 0; i<NUM_LEDS; i++) {
    n = analogRead(MIC_PIN);                                   // Raw reading from mic
    n = abs(n - 512 - DC_OFFSET);                              // Center on zero
    leds[i] = CHSV((n*2 % 255), 255, (n*2)% 255);
  }
} // soundmems()
