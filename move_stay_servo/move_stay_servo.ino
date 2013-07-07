#include <Servo.h>

Servo panServo;      
Servo tiltServo;      

int servoPanPosition = 90;
int servoTiltPosition = 90;
int joystickPanPin = A0;
int joystickTiltPin = A1;
int joystickPanSpeed = 0;
int joystickTiltSpeed = 0;
int servoPanPin = 9;
int servoTiltPin = 10;


void setup()
{
  pinMode(servoPanPin, OUTPUT);
  pinMode(servoTiltPin, OUTPUT);
  panServo.attach(servoPanPin);
  tiltServo.attach(servoTiltPin);
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

  panServo.write(servoPanPosition);
  tiltServo.write(servoTiltPosition);

  delay(50);
    // adjustable for overall speed
}
