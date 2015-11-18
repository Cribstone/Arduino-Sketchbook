/*
  Esplora Music

  This sketch turns the Esplora in a simple musical instrument.
  Press the Switch 1 and move the slider to see how it works.

  Created on 22 november 2012
  By Enrico Gueli <enrico.gueli@gmail.com>
  modified 22 Dec 2012
  by Tom Igoe
*/


#include <Esplora.h>

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() { // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    Esplora.tone(8, melody[thisNote], noteDuration);
    
      // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  } 
}

void loop() {
 
}
