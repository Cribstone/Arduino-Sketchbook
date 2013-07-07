// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.
 
// include the TinkerKit library
#include <TinkerKit.h>
 
// include the Servo library
#include <Servo.h> 
  
Servo TKMicroServo;  // create servo object to control a servo 
                     // a maximum of eight servo objects can be created 
  
int pos = 0;    // variable to store the servo position 
  
void setup() 
{
  TKMicroServo.attach(5);  // attaches the servo on Output 0 of the TinkerKit Shield to the servo object 
}

void loop() 
{
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    TKMicroServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    TKMicroServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);       // waits 15ms for the servo to reach the position   
  }
}
