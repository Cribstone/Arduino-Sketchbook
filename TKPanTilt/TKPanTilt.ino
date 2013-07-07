
#include <TinkerKit.h>
#include <Servo.h> 
  
Servo TKMicroServo;       
Servo TKContinuousRotationServo;

int servoPanPosition = 90;
int servoTiltPosition = 90;
int joystickPanPin = A0;
int joystickTiltPin = A1;
int joystickPanSpeed = 0;
int joystickTiltSpeed = 0;
int servoPanPin = 6;
int servoTiltPin = 5;


void setup() 
{ 
  pinMode(servoPanPin, OUTPUT);
  pinMode(5, OUTPUT);
  TKContinuousRotationServo.attach(servoPanPin);
  TKMicroServo.attach(servoTiltPin);
}

void loop() 
{
  joystickPanSpeed = (analogRead(joystickPanPin) - 512) / 50;
    // -512 to provide equal +/- numbers
  joystickTiltSpeed = (analogRead(joystickTiltPin) - 512) / -50;
    // negative 50 to reverse direction

  servoPanPosition = constrain((servoPanPosition + joystickPanSpeed), 1, 180);
  servoTiltPosition = constrain((servoTiltPosition + joystickTiltSpeed), 1, 180);
    // constarin function to not exceed servo limits

  TKContinuousRotationServo.write(servoPanPosition);
  TKMicroServo.write(servoTiltPosition);

  delay(50);
    // adjustable for overall speed
}
