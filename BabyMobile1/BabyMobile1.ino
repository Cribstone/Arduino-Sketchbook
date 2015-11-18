#include <pitches.h>

// Define the pin numbers.
const int inputButton = 0;
const int inputMotion = A0;
const int inputSound = A1;
const int outputMotor = 1;
const int outputBargraph = 5;
const int outputSpeaker = 9;

// Declare variables.
int soundLevel;
int motionLevel;
unsigned long lastTrigger;
unsigned long curTrigger;

// Define the Brahms' Lullaby song.
int notes [] = { N_E4, N_E4, N_G4, N_E4, N_E4, N_G4, N_REST,
                 N_E4, N_G4, N_C5, N_B4, N_A4, N_A4, N_G4,
                 N_D4, N_E4, N_F4, N_D4, N_D4, N_E4, N_F4, N_REST,
                 N_D4, N_F4, N_B4, N_A4, N_G4, N_B4, N_C5, N_REST,
                 N_C4, N_C4, N_C5, N_A4, N_F4, N_G4,
                 N_E4, N_C4, N_F4, N_G4, N_A4, N_E4, N_G4,
                 N_C4, N_C4, N_C5, N_A4, N_F4, N_G4,
                 N_E4, N_C4, N_F4, N_G4, N_F4, N_E4, N_D4, N_C4 };
                      
int numNotes = (sizeof(notes)/sizeof(int));

// Quarter note = 250, Eighth = 125, Quarter with a dot (1/4 + 1/8) = 375, etc.
int durations [] = { 125, 125, 375, 125, 250, 250, 250,
                     125, 125, 250, 375, 125, 250, 250,
                     125, 125, 250, 250, 125, 125, 250, 250,
                     125, 125, 125, 125, 250, 250, 250, 250,
                     125, 125, 500, 125, 125, 500,
                     125, 125, 250, 250, 250, 50, 450,
                     125, 125, 500, 125, 125, 500,
                     125, 125, 200, 25, 25, 250, 250, 500};

void setup(){
  // Initialize the pins.
  pinMode(inputButton, INPUT);
  pinMode(inputMotion, INPUT);
  pinMode(inputSound, INPUT);
  pinMode(outputMotor, OUTPUT);
  pinMode(outputBargraph, OUTPUT);
  pinMode(outputSpeaker, OUTPUT);
  
  // Start the timer.
  lastTrigger = millis();
  
  // Sleep for ten seconds. Use lights to indicate.
  // This is so we can walk away without triggering the sensors.
  analogWrite(outputBargraph, 255); delay(10000);
  analogWrite(outputBargraph, 0);
}

void loop(){
  // If the button is pressed, run the mobile for a while.
  if(digitalRead(inputButton) == true){
    runMobile(10);
    delay(10000);
  }
  
  // Read the input triggers.
  soundLevel = analogRead(inputSound);
  motionLevel = analogRead(inputMotion);
  
  // If the sound or motion triggers are activated and the last time it
  // was activated is less than ten seconds ago, the baby is fussing.
  if(soundLevel > 1 || motionLevel > 1022){
    // Set the trigger time.
    curTrigger = millis();
    
    // Toggle the lights for a quarter second to show a trigger.
    analogWrite(outputBargraph, 255); delay(250);
    analogWrite(outputBargraph, 0);
    
    if((curTrigger - lastTrigger) < 10000){
      runMobile(5);
      delay(10000);
    }
    
    // Update the timer.
    lastTrigger = curTrigger;
    
    // Wait before taking the next reading.
    // This prevents one cry from possibly
    // being considered two separate triggers.
    delay(3000);
  }
}

void runMobile(int numLoops){
  // Start the motor.
  digitalWrite(outputMotor, HIGH);
  
  // Play the song.
  playSong(numLoops);
  
  // Stop the motor.
  digitalWrite(outputMotor, LOW);
}

// Play the song, and toggle the lights on and off with each tone.
void playSong(int numLoops){
  for(int loops = 0; loops < numLoops; loops++){
    for(int i = 0; i < numNotes; i++){
      // Turn on the lights if it isn't a rest.
      if(notes[i] != N_REST){ analogWrite(outputBargraph, 255); }
      
      // Play the note for the specified duration.
      // Times two to slow it down to 3/4 time.
      tone(outputSpeaker, notes[i], durations[i] * 2);
      
      // Pause until the next note.
      delay(round(durations[i]));
      
      // Turn off the lights.
      analogWrite(outputBargraph, 0);
      
      // More delay.
      delay(round(durations[i] * 1.33));
    }
  }
