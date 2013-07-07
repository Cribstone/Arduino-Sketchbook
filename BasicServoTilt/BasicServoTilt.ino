#include <Servo.h>

Servo servoPan;

Servo servoTilt;

void setup() { 
                 servoPan.attach(9); // pan servo is on pin 9
                 servoTilt.attach(10); // tilt servo is on pin 10
                 servoPan.write(90); // home both servos to center
                 servoTilt.write(90); // home both servos to center
                }

void loop() {
                 if( Serial.available() >= 2 ) { // two bytes waiting for us
                 int pan = Serial.read(); // 1st byte is Pan position
                 int tilt = Serial.read(); // 2nd byte is Tilt position
                 servoPan.write(pan); // move pan servo
                 servoTilt.write(tilt); // move tilt servo
                 }
                }
