/*
By: Peter Routon
Date: December 2014
This is my adaptation and simplification (i.e., reduced options) of meteorShower, Jon Burroughs' adaptation
of some funkboxing code from Thomas Eldridge.
*/
//================================================
#include "FastLED.h"
#define LED_COUNT 33
#define LED_PIN 0
struct CRGB leds[LED_COUNT];
//================================================

uint8_t hue = 32;
byte idex = 255;
byte meteorLength = 29;

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);
  LEDS.addLeds<WS2812, LED_PIN, GRB>(leds, LED_COUNT);
  LEDS.setBrightness(200);
}

void loop(){
 meteorShower(); 
}

void meteorShower(){
  // slide all the pixels down one in the array
  memmove8( &leds[1], &leds[0], (LED_COUNT - 1) * 3 );

  // increment the meteor display frame
  idex++;
  // make sure we don't drift into space
  if ( idex > meteorLength ) {
    idex = 0;
    // cycle through hues in each successive meteor tail
    hue += 32;  
  }

  // this switch controls the actual meteor animation, i.e., what gets placed in the
  // first position and then subsequently gets moved down the strip by the memmove above
  switch ( idex ) {
  case 0:
    leds[0] = CRGB(200,200,200); 
    break;
  case 1:
    leds[0] = CHSV((hue - 20), 255, 210); 
    break;
  case 2:
    leds[0] = CHSV((hue - 22), 255, 180); 
    break;
  case 3:
    leds[0] = CHSV((hue - 23), 255, 150); 
    break;
  case 4:
    leds[0] = CHSV((hue - 24), 255, 110); 
    break;
  case 5:
    leds[0] = CHSV((hue - 25), 255, 90); 
    break;
  case 6:
    leds[0] = CHSV((hue - 26), 160, 60); 
    break;
  case 7:
    leds[0] = CHSV((hue - 27), 140, 40); 
    break;
  case 8:
    leds[0] = CHSV((hue - 28), 120, 20); 
    break;
  case 9:
    leds[0] = CHSV((hue - 29), 100, 20); 
    break;
  default:
    leds[0] = CRGB::Black; 
  }

  // show the blinky
  FastLED.show();  
  // control the animation speed/frame rate
  delay(30);
}
