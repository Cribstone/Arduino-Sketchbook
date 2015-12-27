/*
Arduino Hardware - SparkFun RGB Rotary Encoder

Lyndon Kroker, 23-Nov-2014

This code has very nice stable output and there is no need for any kind of 
hardware debounce.  All of the variables and constants begin with "enc".  
This should make it easier to include the code into an existing sketch.  
Two encoders could be supported by changing the "enc" part of the variable 
name to "enc1" and "enc2".  Copy the existing encProcess() function and 
rename it to something like enc2process().  Add the required constant and 
varibles to the beginning of the sketch and don't forget to update the 
varible names in your new function.

If you change the pins and still want all of the sample colors to work, 
don't forget to use pins that have PWM output capability.

This example uses the ATmega's interal pullup resistors so you don't need 
to connect any resistors to the A or B pins of the encoder.  Connect the 
C (Common) pin directly to GND.  When the encoder is at rest, both pin A 
and B are HIGH.

I used 330 Ohm resistors for the encoders LEDs and a 10K pulldown resistor 
for the pushbutton switch.  (The pushbutton switch will output 5V when 
pushed).  The switch is not designed to drive a load so don't connect any 
load directly to it.  Use a transistor or MOSFET.

This code was based on code originally written by TETRASTYLE (Aug.7.2012).
Project page: http://dev.tetrastyle.net/2012/08/blog-post.html
Original code: http://djgj.sub.jp/TETRASTYLE/arduino/rot_enc_rgb.txt

*/



/* constants and variables */
const int encPinA = 2;         // input pins
const int encPinB = 4;         //
const int encSwitchPin = 7;    //

const int encRedLED = 3;       // output pins
const int encGreenLED = 5;     //
const int encBlueLED = 6;      //

const int encRight = 1;            // don't change these values
const int encLeft = 2;             //
const int encNoChange = 3;         //
int encValueA = 0;                 //
int encValueB = 0;                 //
int encDirection = encNoChange;    //

int encValue = 128;      // this is the starting value
int encMode = 0;         // this is the starting color mode (red)
int encOldValue = -1;    //
int encOldMode = -1;     //

const int encMinValue = 0;      // change these if you wish
const int encMaxValue = 255;    //
const int encStep = 1;          //



/* program setup */
void setup() {

  pinMode(encPinA, INPUT_PULLUP);    // active LOW logic
  pinMode(encPinB, INPUT_PULLUP);    // active LOW logic
  pinMode(encSwitchPin, INPUT);      // active HIGH logic (not pullup)

  Serial.begin(57600);
  Serial.println("Rotary encoder standing by...");
}



/* main program */
void loop() {

  encProcess();                    // check for encoder rotation
  encSwitchProcess();              // check for button push

  if(encValue != encOldValue) {    // encoder rotation actions
    Serial.println(encValue);
    encOldValue = encValue;
  }
  
  if(encMode != encOldMode) {      // button push actions
    Serial.print("sw:");
    Serial.println(encMode);
    ledProcess();
    encOldMode = encMode;
  }
}



/* program functions */
void encProcess()
{
  encValueA = digitalRead(encPinA);    // read encoder pins
  encValueB = digitalRead(encPinB);    //
  delay(1);

  if(encValueA == HIGH && encValueB == HIGH) {    // normal resting condition //

    if(encDirection == encRight) {           // right turn detected on previous loop
      encValue += encStep;                   //
      if(encValue > encMaxValue) {           //
        encValue=encMaxValue;                //
      }                                      //
    } else if (encDirection == encLeft) {    // left turn detected on previous loop
      encValue -= encStep;                   //
      if(encValue < encMinValue) {           //
        encValue = encMinValue;              //
      }                                      //
    }                                        //

    encDirection = encNoChange;    // normal resting action //

  } else {

    if(encValueA == LOW) {        // right turn detected
      encDirection = encRight;    //
    }                             //
    if(encValueB == HIGH) {       // left turn detected
      encDirection = encLeft;     //
    }                             //
    
  }
}

void encSwitchProcess()
{
  if(digitalRead(encSwitchPin)) {
    delay(20);
    while(digitalRead(encSwitchPin)) {
      delay(1);
    }
    encMode++;      // increment the mode count
    encMode %=7;    // but don't go over 6
  }
}

void ledProcess()
{
  switch(encMode){
    case 0: //RED
      analogWrite(encRedLED,0);        // looks funny but remember,
      analogWrite(encGreenLED,255);    // we are using active LOW logic
      analogWrite(encBlueLED,255);     //
      break;
    case 1: //GREEN
      analogWrite(encRedLED,255);
      analogWrite(encGreenLED,0);
      analogWrite(encBlueLED,255);
      break;
    case 2: //BLUE
      analogWrite(encRedLED,255);
      analogWrite(encGreenLED,255);
      analogWrite(encBlueLED,0);
      break;          
    case 3: //PINK
      analogWrite(encRedLED,0);
      analogWrite(encGreenLED,255);
      analogWrite(encBlueLED,0);
      break;
    case 4: //YELLOW
      analogWrite(encRedLED,0);
      analogWrite(encGreenLED,0);
      analogWrite(encBlueLED,255);
      break;
    case 5: //SKY BLUE
      analogWrite(encRedLED,255);
      analogWrite(encGreenLED,0);
      analogWrite(encBlueLED,0);
      break;
    case 6: //WHITE
      analogWrite(encRedLED,0);
      analogWrite(encGreenLED,0);
      analogWrite(encBlueLED,0);
      break;
    default: //OFF
      analogWrite(encRedLED,255);
      analogWrite(encGreenLED,255);
      analogWrite(encBlueLED,255);
      break;
  }        
}


