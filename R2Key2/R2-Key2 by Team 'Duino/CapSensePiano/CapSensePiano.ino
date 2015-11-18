/*
  R2-Key2 Piano by Team 'Duino from GWCintuit Class of 2014
 When user touches one of the paper keys, the speaker plays the corresponding tone as the LED strip lights up with the corresponding color.
 All visualization code is done using Processing. 
 
 Project modified from the "Capacitive-Touch Arduino Keyboard Piano" originally posted on Instructables.com
 Version Aug 1, 2014
 */

/*
  Capacitive-Touch Arduino Keyboard Piano
 
 Plays piano tones through a speaker when the user taps touch-sensitive piano "keys"
 
 Created  18 May 2013
 Modified 23 May 2013
 by Tyler Crumpton and Nicholas Jones
 
 This code is released to the public domain. For information about the circuit,
 visit the Instructable tutorial at http://www.instructables.com/id/Capacitive-Touch-Arduino-Keyboard-Piano/
 */

#include <CapacitiveSensor.h>
#include "pitches.h"
#include <toneAC.h>
#include <Adafruit_NeoPixel.h>


#define COMMON_PIN      2    // The common 'send' pin for all keys
#define soundOutput     12   //A4   // The output pin for the piezo buzzer
#define NUM_OF_SAMPLES  10   // Higher number whens more delay but more consistent readings
#define CAP_THRESHOLD   25   // Capactive reading that triggers a note (adjust to fit your needs)
#define NUM_OF_KEYS     8    // Number of keys that are on the keyboard

int portTouching; //recording what key is touched, used for stopping sound output

//for led strip
#define stripPIN 13
#define numPixels
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, stripPIN, NEO_GRB + NEO_KHZ800);
uint32_t color; //for changing color depending on input

// This macro creates a capacitance "key" sensor object for each key on the piano keyboard:
#define CS(Y) CapacitiveSensor(2, Y)

// Each key corresponds to a note, which are defined here. Uncomment the scale that you want to use:
int notes[]={
  NOTE_C5,NOTE_B4,NOTE_A4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C4}; // C-Major scale
//int notes[]={NOTE_A4,NOTE_B4,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_A5}; // A-Minor scale
//int notes[]={NOTE_C4,NOTE_DS4,NOTE_F4,NOTE_FS4,NOTE_G4,NOTE_AS4,NOTE_C5,NOTE_DS5}; // C Blues scale

// Defines the pins that the keys are connected to:
CapacitiveSensor keys[] = {
  CS(3), CS(4), CS(5), CS(6), CS(7), CS(8), CS(11), CS(12)};


//---------------------------------------------------------------

void setup() { 
  Serial.begin(9600);
  // Turn off autocalibrate on all channels:
  for(int i=0; i<8; ++i) {
    keys[i].set_CS_AutocaL_Millis(0xFFFFFFFF);
  }
  // Set the speaker as an output:
  pinMode(soundOutput, OUTPUT); 

  //led strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //strip.setBrightness(100);
}

void loop() {
  int counter = 0;

  // Loop through each key:
  for (int i = 0; i < 8; ++i) {
    // If the capacitance reading is greater than the threshold, play a note:

    if(keys[i].capacitiveSensor(NUM_OF_SAMPLES) > CAP_THRESHOLD) {
      counter++;

      //when two keys are being pressed at the same time, only play the note of the first key
      if (counter == 1)
      {
        //tone(soundOutput, notes[i]); // Plays the note corresponding to the key pressed
        toneAC(notes[i], 10);
        portTouching = i;
        Serial.write(i+1); //transfer data to Processing

        //for light show
        //setting color depending on what key being touched
        if (i == 7)
        {
          color = strip.Color(8, 0, 255); //purple
        }
        else if (i == 6)
        {
          color = strip.Color(0, 0, 255); //blue
        }
        else if (i == 5)
        {
          color = strip.Color(0, 30, 255); //blue-green
        }
        else if (i == 4)
        {
          color = strip.Color(0, 255, 0); //green
        }
        else if (i == 3)
        {
          color = strip.Color(10, 30, 0); //yellow
        }
        else if (i == 2)
        {
          color = strip.Color(15, 15, 0); //orange
        }
        else if (i == 1)
        {
          color = strip.Color(255, 0, 0); //red
        }
        else if (i == 0)
        {
          color = strip.Color(25, 0, 255); //pink
        }

        //turing on each pixel (30 total)
        for (int j = 0; j<30; j++)
        {
          strip.setPixelColor(j, color);
          strip.setBrightness(100);
          strip.show();
        }

      }

    } 
    else if (keys[portTouching].capacitiveSensor(NUM_OF_SAMPLES) <= CAP_THRESHOLD) {
      //noTone(soundOutput);
      noToneAC();
    }
  }
}


