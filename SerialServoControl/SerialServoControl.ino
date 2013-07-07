/*******************************************************
* SerialServoControl Sketch
* Written by Ryan Owens for SparkFun Electronics
* 7/15/11
*
* This sketch listens to serial commands and uses the data
* to set the position of two servos.
*
* Serial Command Structure: 2 bytes - [ID Byte][Servo Position byte]
* ID byte should be 0 or 1.
* Servo position should be a value between 0 and 180.
* Invalid commands are ignored
* The servo position is not error checked.
* 
* Hardware Setup
* Servos should be connected to pins 2 and 3 of the Arduino.
* 9V DC Power supply is recommended as USB can't always handle powering two servos
*/

#include <Servo.h>  //Used to control the Pan/Tilt Servos

//These are variables that hold the servo IDs.
char tiltChannel=0, panChannel=1;

//These are the objects for each servo.
Servo servoTilt, servoPan;

//This is a character that will hold data from the Serial port.
char serialChar=0;

void setup(){
  servoTilt.attach(2);  //The Tilt servo is attached to pin 2.
  servoPan.attach(3);   //The Pan servo is attached to pin 3.
  servoTilt.write(90);  //Initially put the servos both
  servoPan.write(90);      //at 90 degress.
  
  Serial.begin(9600);  //Set up a serial connection for 57600 bps.
}

void loop(){
  while(Serial.available() <=0);  //Wait for a character on the serial port.
  serialChar = Serial.read();     //Copy the character from the serial port to the variable
  if(serialChar == tiltChannel){  //Check to see if the character is the servo ID for the tilt servo
    while(Serial.available() <=0);  //Wait for the second command byte from the serial port.
    servoTilt.write(Serial.read());  //Set the tilt servo position to the value of the second command byte received on the serial port
  }
  else if(serialChar == panChannel){ //Check to see if the initial serial character was the servo ID for the pan servo.
    while(Serial.available() <= 0);  //Wait for the second command byte from the serial port.
    servoPan.write(Serial.read());   //Set the pan servo position to the value of the second command byte received from the serial port.
  }
  //If the character is not the pan or tilt servo ID, it is ignored.
}

