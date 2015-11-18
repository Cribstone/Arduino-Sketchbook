/*
* Play a simple tune using a piezzo 
* Based on example created by Tom Igoe 
* http://arduino.cc/en/Tutorial/Tone
*/

// Make sure the file pitches.h is placed in the same folder as this sketch
#include "pitches.h"

// Piezzo element connected to Arduino pin 12 and ground
const int buzzerPin = 12;

// Array with the notes in the melody (see pitches.h for reference)
int melody[] = {NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4};

// Array with the note durations: a quarter note has a duration of 4, half note 2 etc.
int durations[]  = {4, 4, 4, 5, 16, 4, 5, 16, 2, 4, 4, 4, 5, 16, 4, 5, 16, 2};

int tempo = 120; // tempo for the melody expressed in beats per minute (BPM)

void setup() {
  playTune(melody, durations, tempo);
}

void loop() {
  // no need to repeat the melody.
}

void playTune(int notes[], int durations[], int BPM) 
{
  int tuneSize = sizeof(melody) / sizeof(int);
  
  // iterate over the notes of the tune:
  for (int thisNote = 0; thisNote < tuneSize; thisNote++) {

    // For details on calculating the note duration using the tempo and the note type,
    // see http://bradthemad.org/guitar/tempo_explanation.php.
    // A quarter note at 60 BPM lasts exactly one second and at 120 BPM - half a second.
    
    int noteDuration = (int)((1000 * (60 * 4 / BPM)) / durations[thisNote] + 0.);
    tone(buzzerPin, notes[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 20% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.20;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzerPin);
  }
}
