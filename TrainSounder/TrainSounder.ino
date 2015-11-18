// constants won't change:

const int soundTrigger = A1; //soundtrigger on pin A1:

const int cloud = 5; // cloud on digital pin 5 

                    // speaker on digital pin 9


void setup() {
  
  pinMode (soundTrigger, INPUT); // tells arduino to make pin A1 an input

  pinMode (cloud, OUTPUT);  //tells arduino to make pin D5 an output

}

void loop() {
  
  
  
 while (digitalRead(soundTrigger) == HIGH) { // do nothing till soundtrigger goes high 

  digitalWrite(cloud, HIGH);  // send high signal to cloud

  delay(1500); // makes sure cloud gets good high signal befor playing melody
 
 play_jeopardyMelody();  
  
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

