/*
	
 littleBits Arduino Module
	
 Based on Arduino toneMelody sketch:
	
 littlebits jeopardy theme
	
 created 17 july 2014
	
 
	
 http://arduino.cc/en/Tutorial/Tone
	
 modified for littleBits March 2014 by William Brewer
	
 
	
 _Tone Melody_
	
 
	
   What is supposed to happen:
	
 * Plays the Mary Had A "LittleBits" Lamb theme song using the tone command.
	
 
	
   Circuit:
	
 * LittleBits arduino 
	
 * LittleBits speaker on digital pin 9 (will not work with buzzer)
	
 * littleBits cloud
	
 * LittleBits sound trigger
	
 
	
 This sketch includes the following commands:
	
 while();
	
 delay(time);                                   time: milliseconds

 noTone();                                      ends all tones. Must use between tone commands

 tone(pin, frequency, duration);                pin: digital pin 9

                                                frequency: the frequency of the tone in hertz

                                                duration: the duration of the tone in milliseconds

 

 Additional Notes:

 This sketch also includes two integer arrays named "jeopardy" and "jeopardyDurations".

 Arrays allow you to store multiple values of the same datatype under one name.

 This functionality combined with a for loop makes it easy to write code that refernces a lot of values.

 

 */



#include "pitches.h"  // tells sketch that library "pitches.h" has information to be used 



// notes in mary had a littleBits lamb:

  int jeopardy[] = {

    NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,

  NOTE_B4, NOTE_B4, NOTE_B4,

  NOTE_A4, NOTE_A4, NOTE_A4,

  NOTE_B4, NOTE_D5, NOTE_D5,

  NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4,

  NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4,

  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4,

  NOTE_G4,
  
	
  //twinkle twinkle littleBits star

  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4,

  NOTE_A4, NOTE_A4, NOTE_G4,

  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,

  NOTE_D4, NOTE_D4, NOTE_C4,

  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4,

  NOTE_E4, NOTE_E4, NOTE_D4,

  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4,

  NOTE_E4, NOTE_E4, NOTE_D4,
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_G4,
	
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
	
  NOTE_D4, NOTE_D4, NOTE_C4,
	
 
	
 /*notes in the Jeopardy melody:


  NOTE_C5, NOTE_F5, NOTE_C5, NOTE_F4, NOTE_A4, NOTE_C5, NOTE_F5, NOTE_C5,
	
  NOTE_C5, NOTE_F5, NOTE_C5, NOTE_F5,
	
  NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_CS5,
	
  NOTE_C5, NOTE_F5, NOTE_C5, NOTE_F4, NOTE_A4, NOTE_C5, NOTE_F5, NOTE_C5,

  NOTE_F5, NOTE_D5, NOTE_C5, NOTE_AS4,

  NOTE_A4, NOTE_G4, NOTE_F4

*/

};



// note durations: 4 = quarter note, 8 = eighth note, etc.:

int jeopardyDurations[] = {

  4, 4, 4, 4,
	
  4, 4, 2,

  4, 4, 2,

  4, 4, 2, 

  4, 4, 4, 4,

  4, 4, 4, 4,

  4, 4, 4, 4,

  1,

  

  //twinkle twinkle littleBits Star note duration:

  

  4,4,4,4,

  4,4,2,

  4,4,4,4,

  4,4,2,

  4,4,4,4,

  4,4,2,

  4,4,4,4,

  4,4,2,

  4,4,4,4,

  4,4,2,

  4,4,4,4,

  4,4,2,

  

  /* Jeopardy note durations

  4, 4, 4, 8, 8, 4, 4, 2,

  4, 4, 4, 4,

  3, 8, 8, 8, 8, 8,

  4, 4, 4, 8, 8, 4, 4, 2,

  3, 8, 4, 4,

  4, 4, 2 

*/

};

// constants won't change:

const int soundTrigger = A1; //soundtrigger on pin A1:

const int cloud = 5; // cloud on digital pin 5 

                    // speaker on digital pin 9


void setup()

{

  pinMode (soundTrigger, INPUT); // tells arduino to make pin A1 an input

  pinMode (cloud, OUTPUT);  //tells arduino to make pin D5 an output

}









void loop()  {

  



 
 

 while (digitalRead(soundTrigger) == HIGH) { // do nothing till soundtrigger goes high 

   digitalWrite(cloud, HIGH);  // send high signal to cloud

   delay(1500); // makes sure cloud gets good high signal befor playing melody

  play_jeopardyMelody();  // 

 }

}




void play_jeopardyMelody(){

  // iterate over the notes of the melody:

  for (int thisNote = 0; thisNote < (sizeof(jeopardyDurations)/2); thisNote++) {



    // to calculate the note duration, take one second 

    // divided by the note type.

    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int jeopardyDuration = 1000/jeopardyDurations[thisNote];

    tone(9, jeopardy[thisNote], jeopardyDuration);



    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = jeopardyDuration * 1.30;
	
    delay(pauseBetweenNotes);

    

    noTone(9); // stop the tone playing:

    digitalWrite(cloud, LOW); // turns signal off 

  }

}

