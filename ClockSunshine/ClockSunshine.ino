#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <TimeAlarms.h>
#include <FastLED.h>

#define NUM_LEDS 12
#define DATA_PIN 6

CRGB leds[NUM_LEDS];
CRGB OffColour = CRGB(0, 1, 2);
CRGB OnColour = CRGB(16, 8, 0);;

void setup()  
{
  //Create a time struct with the alarm time
  tmElements_t tm;
  tm.Hour = 7;
  tm.Minute = 0;
  tm.Second = 0;
  
  // Get the time from the rtc
  setSyncProvider(RTC.get);   
   
  // Initialise the LEDs in the ring
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  //Used to test by setting the time to now + 5 seconds
  RTC.read(tm);
  tm.Second += 5;
  
  //Create an alarm for the morning
  Alarm.alarmOnce(tm.Hour, tm.Minute, tm.Second, LightsOn);  
    
  //Create an alarm an hour later to turn the lights off again
  Alarm.alarmOnce(tm.Hour + 1, tm.Minute, tm.Second, LightsOff);  
  
  // Set the initial colour of the LED's
  LightsOff();
}

void loop()
{
  // Alarm.delay has to be called occasionally to make the alarms work
  Alarm.delay(500);
}

//Create a sunrise style effect
void LightsOn()
{
  for(int i = 0; i < NUM_LEDS / 2; i += 1)
  {  
    leds[i] = OnColour;
    leds[NUM_LEDS - i - 1] = OnColour;

    FastLED.show();
    Alarm.delay(500);
  }
}

//Set the LED's all to the off colour
void LightsOff()
{
  for(int i = 0; i < NUM_LEDS; i += 1)
    leds[i] = CRGB(0, 1, 2);

  FastLED.show();
}


