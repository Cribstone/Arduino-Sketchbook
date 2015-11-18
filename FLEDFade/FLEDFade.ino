/*
Put together & tested by:
Scottie Digital
 */
#include "FastLED.h"
#define NUM_LEDS 12  // # of LEDS in the strip
CRGB leds[NUM_LEDS];
#define PIN 6 // Output Pin to Data Line on Strip
#define COLOR_ORDER GRB  // I had to change this for my strip if your color is off then you know.
int fadeAmount = 5;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
int brightness = 0; 

void setup()
{
  FastLED.addLeds<WS2812, PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
}

void loop()
{ 
   for(int i = 0; i < NUM_LEDS; i++ )
   {
   leds[i].setRGB(0,255,250);  // Set Color HERE!!!
   leds[i].fadeLightBy(brightness);
  }
  FastLED.show();
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade: 
  if(brightness == 0 || brightness == 255)
  {
    fadeAmount = -fadeAmount ; 
  }    
  delay(65);  // This delay sets speed of the fade. I usually do from 5-75 but you can always go higher.
}
