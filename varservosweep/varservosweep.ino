/ speed controlled Servo Sweep
// see: http://arduino.cc/forum/index.php/topic,61586.0.html
//
// Michael Margolis 4 August 2011
// edited by http://www.bajdi.com

#include <VarSpeedServo.h>

VarSpeedServo MyServo;        // servo objects

int servoSpeeds = 30; // sweep speed, 1 is slowest, 255 fastest)
int servoMinPosition = 0; // the minumum servo angle
int servoMaxPosition = 180; // the maximum servo angle

void setup()
{

  MyServo.attach(9);       
  MyServo.slowmove(servoMinPosition,servoSpeeds) ; // start sweeping from min position

}

void loop()
{
  // sweep the servos

  if( MyServo.read() == servoMinPosition)
  {
    MyServo.slowmove(servoMaxPosition,servoSpeeds) ; 
  }       
  else if( MyServo.read() == servoMaxPosition)
  {
    MyServo.slowmove(servoMinPosition,servoSpeeds) ; 
  }       

}
