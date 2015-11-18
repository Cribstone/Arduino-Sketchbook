// If using different pins on your board, change them here. 
int key1 = A2; // Brahms Lullaby
int key2 = 11; // Twinkle Twinkle
int key3 = 10; //Clair de Lune
int key4 = 9; // The Cuckoo

//Notes are designated for each song. For example, C4 means middle C. For Clair De Lune, only notes are given, not places on the keyboard. 

int speaker = 5; //This means your buzzer is connected to pin 5 on the Protoboard.If you used another pin, change it. 
int sensorValue1;
int sensorValue2;
int sensorValue3;
int sensorValue4;

// the setup routine runs once when you press reset: Leave this code in. 
void setup() {
    pinMode(speaker, OUTPUT);
    pinMode(key1, INPUT);
    pinMode(key2, INPUT);
    pinMode(key3, INPUT);
    pinMode(key4, INPUT);
    Serial.begin(9600); // initialize the communication
}


// the loop routine runs over and over again forever: 
void loop() 
{ //If you want to change songs, here is where you would do it. You would want to delete everyting from //Brahms Lullaby to right before the final bracket. This process is followed for all other songs.
  sensorValue1 = readCapacitivePin(key1); //read the touch sensor value
  Serial.println(sensorValue1); //send value to the computer
  delay(100); //delay for 1/10 of a second
  if(sensorValue1 > 2)
  {
    // Brahms Lullaby

//Start with E4

    tone(speaker, 329.63, 300); //E4
    delay(300);
tone(speaker, 329.63, 300); //E4
delay(300);
tone(speaker, 392.00, 900); // G4
delay(900);
tone(speaker, 329.63, 200);// E4
delay(200);
tone(speaker, 329.63, 300); // E4
delay(300);
tone(speaker, 392.00, 600); // G4
delay(600);
tone(speaker, 329.63, 300); // E4
delay(300);
tone(speaker, 392.00, 300); // G4
delay(300);
tone(speaker, 523.25, 300); // C5
delay(300);
tone(speaker, 493.88, 500); // B4
delay(500);
tone(speaker, 440.00, 200); // A4
delay(200);
tone(speaker, 440.00, 300); // A4
delay(300);
tone(speaker, 392.00, 300); // G4
delay(300);
tone(speaker, 293.66, 300); // D4
delay(300);
tone(speaker, 329.63, 300); //E4
delay(300);
tone(speaker, 349.23, 300); // F4
delay(300);
tone(speaker, 293.66, 300);// D4
delay(300);
tone(speaker, 293.66, 300); //D4
delay(300);
tone(speaker, 329.63, 300); // E4
delay(300);
tone(speaker, 349.23, 600); // F4
delay(600);
tone(speaker, 293.66, 300);// D4
delay(300);
tone(speaker, 349.23, 300); // F4
delay(300);
tone(speaker, 493.88, 300); // B4
delay(300);
tone(speaker, 440.00, 300); // A4
delay(300);
tone(speaker, 392.00, 300); // G4
delay(300);
tone(speaker, 493.88, 300); // B4
delay(300);
tone(speaker, 523.25, 600); // C4
delay(600);

           
  }
       
  sensorValue2 = readCapacitivePin(key2); //read the touch sensor value 
  Serial.println(sensorValue2); //send value to the computer
  delay(100); //delay for 1/10 of a second
  if  (sensorValue2 > 2)
  {  
         // Twinkle Twinkle 

//Start with C4

tone(speaker, 261.63, 600); //C4
delay(600);
tone(speaker, 261.63, 600); //C4
delay(600);
tone(speaker, 392.00, 600); //G4
delay(600);
tone(speaker, 392.00, 600); //G4
delay(600);
tone(speaker, 440.00, 600); //A4
delay(600);
tone(speaker, 440.00, 600); //A4
delay(600);
tone(speaker, 392.00, 600); //G4
delay(600);
tone(speaker, 349.23, 600); //F4
delay(600);
tone(speaker, 349.23, 600); //F4
delay(600);
tone(speaker, 329.63, 600); //E4
delay(600);
tone(speaker, 329.63, 600); //E4
delay(600);
tone(speaker, 293.66, 600); //D4
delay(600);
tone(speaker, 293.66, 600); //D4
delay(600);
tone(speaker, 261.63, 1200); //C4
delay(1200);
tone(speaker, 783.99, 600); //G5
delay(600);
tone(speaker, 783.99, 600); //G5
delay(600);
tone(speaker, 698.46, 600); //F5
delay(600);
tone(speaker, 698.46, 600); //F5
delay(600);
tone(speaker, 659.26, 600); //E5
delay(600);
tone(speaker, 659.26, 600); //E5
delay(600);
tone(speaker, 587.33, 600); //D5
delay(600);
tone(speaker, 783.99, 600); //G5
delay(600);
tone(speaker, 783.99, 600); //G5
delay(600);
tone(speaker, 698.46, 600); //F5
delay(600);
tone(speaker, 698.46, 600); //F5
delay(600);
tone(speaker, 659.26, 600); //E5
delay(600);
tone(speaker, 659.26, 600); //E5
delay(600);
tone(speaker, 587.33, 600); //D5
delay(600);
tone(speaker, 261.63, 600); //C4
delay(600);
tone(speaker, 261.63, 600); //C4
delay(600);
tone(speaker, 392.00, 600); //G4
delay(600);
tone(speaker, 392.00, 600); //G4
delay(600);
tone(speaker, 440.00, 600); //A4
delay(600);
tone(speaker, 440.00, 600); //A4
delay(600);
tone(speaker, 392.00, 600); //G4
delay(600);
tone(speaker, 349.23, 600); //F4
delay(600);
tone(speaker, 349.23, 600); //F4
delay(600);
tone(speaker, 329.63, 600); //E4
delay(600);
tone(speaker, 329.63, 600); //E4
delay(600);
tone(speaker, 293.66, 600); //D4
delay(600);
tone(speaker, 293.66, 600); //D4
delay(600);
tone(speaker, 261.63, 1200); //C4
delay(1200);


   }
      


  sensorValue3 = readCapacitivePin(key3); //read the touch sensor value C
  Serial.println(sensorValue3); //send value to the computer
  delay(100); //delay for 1/10 of a second
  if (sensorValue3 >= 2)
  
  // Clair De Lune
  {
    tone(speaker, 880.00, 900); //A Repeat Start
    delay(900);
    tone(speaker, 739.99, 900); //F#
    delay(900);
    tone(speaker, 659.26, 300); //E
    delay(300);
    tone(speaker, 739.99, 300); //F#
    delay(300);
    tone(speaker, 659.26, 1200); //E
    delay(1200);
    tone(speaker, 587.33, 300); //D // 
    delay(300);
    tone(speaker, 659.26, 300); //E
    delay(300);
    tone(speaker, 587.33, 900); //D
    delay(900);
    tone(speaker, 739.99, 300); //F#
    delay(300);
    tone(speaker, 587.33, 300); //D
    delay(300);
    tone(speaker, 554.37, 300); //C#
    delay(300);
    tone(speaker, 587.33, 300); //D
    delay(300);
    tone(speaker, 554.37, 1200); //C#
    delay(1200);
    tone(speaker, 493.88, 300); //B Repeats to C
    delay(300);
    tone(speaker, 554.37, 300);  //C#
    delay(300);
    tone(speaker, 493.88, 300); //B
    delay(300);
    tone(speaker, 659.36, 300); //E
    delay(300);
    tone(speaker, 493.88, 300); //B
    delay(300);
    tone(speaker, 440.00, 300); //A
    delay(300);
    tone(speaker, 392.00, 300);// G Repeats again at the bottom
    delay(300);
    tone(speaker, 440.00, 300); // A
    delay(300);
    tone(speaker, 392.00, 900); // G
    delay(900);
    tone(speaker, 369.99, 900); // F#
    delay(900);
    tone(speaker, 369.99, 300);// (F#) 1 count
    delay(300);
    tone(speaker, 392.00, 300); // (G) 1 count
    delay(300);
    tone(speaker, 369.99, 300); //(F#) 1 count
    delay(300);
    tone(speaker, 493.88, 300); //(B) 1 count
    delay(300);
    tone(speaker, 369.99, 300); // (F#) 1 count
    delay(300);
    tone(speaker, 329.63, 300); //(E) 1 count)
    delay(300);
    tone(speaker, 369.99, 300); // (F#) 1 count
    delay(300);
    tone(speaker, 329.63, 300); //(E) 1 count)
    delay(300);
    tone(speaker, 293.66, 300); //(D) 1 count
    delay(300);
    tone(speaker, 329.63, 300);// (E) 1 count)
    delay(300);
    tone(speaker, 293.66,900);// (D) 3 count
    delay(900);
    tone(speaker, 277.18, 900);// (C#) 3 count Repeat End
    delay(900);
    tone(speaker, 739.99, 300); // F#;
    delay(300);
    tone(speaker, 880.00, 300); // A 
    delay(300); // Repeat Starts
      tone(speaker, 880.00, 900); //A Repeat Start
    delay(900);
    tone(speaker, 739.99, 900); //F#
    delay(900);
    tone(speaker, 659.26, 300); //E
    delay(300);
    tone(speaker, 739.99, 300); //F#
    delay(300);
    tone(speaker, 659.26, 1200); //E
    delay(1200);
    tone(speaker, 587.33, 300); //D // 
    delay(300);
    tone(speaker, 659.26, 300); //E
    delay(300);
    tone(speaker, 587.33, 900); //D
    delay(900);
    tone(speaker, 739.99, 300); //F#
    delay(300);
    tone(speaker, 587.33, 300); //D
    delay(300);
    tone(speaker, 554.37, 300); //C#
    delay(300);
    tone(speaker, 587.33, 300); //D
    delay(300);
    tone(speaker, 554.37, 1200); //C#
    delay(1200);
    tone(speaker, 493.88, 300); //B Repeats to C
    delay(300);
    tone(speaker, 554.37, 300);  //C#
    delay(300);
    tone(speaker, 493.88, 300); //B
    delay(300);
    tone(speaker, 659.36, 300); //E
    delay(300);
    tone(speaker, 493.88, 300); //B
    delay(300);
    tone(speaker, 440.00, 300); //A
    delay(300);
    tone(speaker, 392.00, 300);// G Repeats again at the bottom
    delay(300);
    tone(speaker, 440.00, 300); // A
    delay(300);
    tone(speaker, 392.00, 900); // G
    delay(900);
    tone(speaker, 369.99, 900); // F#
    delay(900);
    tone(speaker, 369.99, 300);// (F#) 1 count
    delay(300);
    tone(speaker, 392.00, 300); // (G) 1 count
    delay(300);
    tone(speaker, 369.99, 300); //(F#) 1 count
    delay(300);
    tone(speaker, 493.88, 300); //(B) 1 count
    delay(300);
    tone(speaker, 369.99, 300); // (F#) 1 count
    delay(300);
    tone(speaker, 329.63, 300); //(E) 1 count)
    delay(300);
    tone(speaker, 369.99, 300); // (F#) 1 count
    delay(300);
    tone(speaker, 329.63, 300); //(E) 1 count)
    delay(300);
    tone(speaker, 293.66, 300); //(D) 1 count
    delay(300);
    tone(speaker, 329.63, 300);// (E) 1 count)
    delay(300);
    tone(speaker, 293.66,900);// (D) 3 count
    delay(900);
    tone(speaker, 277.18, 1200);// (C#) 4 count Repeat End
    delay(1200);
    
  }
  
  
   sensorValue4 = readCapacitivePin(key4); //read the touch sensor value
  Serial.println(sensorValue4); //send value to the computer
  delay(100); //delay for 1/10 of a second
  if (sensorValue4 >= 2)
  {
    // The Cuckoo

//Start with G5

tone(speaker, 783.99, 600); // G5
delay(600);
tone(speaker, 659.26, 300); //E5
delay(300);
tone(speaker, 783.99, 600); // G5
delay(600);
tone(speaker, 659.26, 300); //E5
delay(300);
tone(speaker, 587.33, 300);//D5
delay(300);
tone(speaker, 523.25, 300);//C5
delay(300);
tone(speaker, 587.33, 300);//D5
delay(300);
tone(speaker, 659.26, 600); //E5
delay(600);
tone(speaker, 523.25, 300);//C5
delay(300);
tone(speaker,783.99, 600); //G5
delay(600);
tone(speaker, 659.26, 300); //E5
delay(300);
tone(speaker,783.99, 600); //G5
delay(600);
tone(speaker, 659.26, 300); //E5
delay(300);
tone(speaker, 587.33, 300);//D5
delay(300);
tone(speaker, 659.26, 300);//E5
delay(300);
tone(speaker, 587.33, 300);//D5
delay(300);
tone(speaker,523.25,600);//C5
delay(600);
tone(speaker,587.33,300);//D5
delay(300);
tone(speaker,587.33,300);//D5
delay(300);
tone(speaker, 659.26, 300);//E5
delay(300);
tone(speaker, 698.46, 600);//F5
delay(600);
tone(speaker, 587.33, 300);//D5
delay(300);
tone(speaker, 659.26, 300);//E5
delay(300);
tone(speaker, 659.26, 300);//E5
delay(300);
tone(speaker, 698.46, 300);//F5
delay(300);
tone(speaker, 783.99, 600);//G5
delay(600);
tone(speaker,659.26,300);//E5
delay(300);
tone(speaker, 783.99, 600);//G5
delay(600);
tone(speaker,659.26,300);//E5
delay(300);
tone(speaker,783.99, 600); //G5
delay(600);
tone(speaker, 659.26, 300); //E5
delay(300);
tone(speaker, 587.33, 300);//D5
delay(300);
tone(speaker, 659.26, 300);//E5
delay(300);
tone(speaker, 587.33, 300);//D5
delay(300);
tone(speaker,523.25,600);//C5
delay(600);
  }  
  
 }


    

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
//  #include "pins_arduino.h" // Arduino pre-1.0 needs this
uint8_t readCapacitivePin(int pinToMeasure) 
{
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}