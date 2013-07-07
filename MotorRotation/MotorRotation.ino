
/*
Move Continuous Rotation Servo
 
// based on <http://arduino.cc/en/Tutorial/Sweep>
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.
 
modified Jan 2013
by Markus Loder-Taucher
*/
 
// include the TinkerKit library
#include <TinkerKit.h>
  
// include the Servo library
#include <Servo.h> 
   
Servo TKContinuousRotationServo;  // create servo object to control a servo 
                                  // a maximum of eight servo objects can be created 
   
int pos = 0;    // variable to store the servo position

void setup() 
{
  TKContinuousRotationServo.attach(5);  // attaches the servo on pin 9 to the servo object  
}

void loop() 
{
   for(pos = 90; pos > 1; pos -= 1)               // servo goes from not moving (90) to full speed in one direction (0)
  {                                              // decreasing the value by 1 each loop 
    TKContinuousRotationServo.write(pos);        // tell servo to go to position in variable 'pos' 
    delay(25);                                   // waits 25 ms for the servo 
  } 
  for(pos = 0; pos < 90; pos += 1)               // servo goes from moving full speed in one direction (0) to not moving (90)
  {                                              // increasing the value by 1 each loop 
    TKContinuousRotationServo.write(pos);        // tell servo to go to position in variable 'pos' 
    delay(25);                                   // waits 25 ms for the servo 
  }
 
  // change direction
 
  for(pos = 90; pos < 180; pos += 1)             // servo goes from not moving (90) to full speed in the other direction (180)
  {                                              // decreasing the value by 1 each loop 
    TKContinuousRotationServo.write(pos);        // tell servo to go to position in variable 'pos' 
    delay(25);                                   // waits 25 ms for the servo 
  } 
  for(pos = 180; pos > 90; pos -= 1)             // servo goes from moving full speed in the other direction (180) to not moving (90)
  {                                              // increasing the value by 1 each loop 
    TKContinuousRotationServo.write(pos);        // tell servo to go to position in variable 'pos' 
    delay(25);                                   // waits 25 ms for the servo 
  }
}
