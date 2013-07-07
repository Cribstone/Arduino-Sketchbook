/*
Pan/Tilt Webcam script with 2 servos
Created by Aneal Khimani
http://anealkhimani.com
2/12/10
*/
#include <Servo.h> //Include the Servo library

Servo pan; //Create pan servo object
Servo tilt; //Create tilt servo object

int panposition = 0; //Variable to determine pan position
int tiltposition = 0; //Variable to determine tilt position

void setup() {

Serial.begin(9600); // initialize serial communication:

// initialize the Servo pins:
pan.attach(9); //Pan Servo attached to pin 9
tilt.attach(10); //Tilt Servo attached to pin 10

}

void loop() {

// read the serial port:
if (Serial.available() > 0) {
int inByte = Serial.read();

/* The PHP script that accompanies this Arduino sketch will send
a byte (ascii) for the characters 'a', 'b', 'c', or 'd'.  Based
on what character is sent, the switch statement below will test
the appropriate servo's current position and either move it, or not
*/

switch (inByte) {
case 'a':    //Pan Left for 'a'
if (panposition < 100){
panposition = (panposition + 10);
tilt.write(panposition);
delay(15);
}
break;

case 'b': //Pan Right for 'b'
if (panposition > 0){
tiltposition = (panposition - 10);
tilt.write(panposition);
delay(15);
}
break;

case 'd':  //Tilt Up for 'd'
if (tiltposition < 100){
tiltposition = (tiltposition + 10);
pan.write(tiltposition);
delay(15);
}
break;

case 'c':  //Tilt Down for 'c'
if (tiltposition > 0) {
tiltposition = (tiltposition - 10);
pan.write(tiltposition);
delay(15);
}
break;
}
}
}
