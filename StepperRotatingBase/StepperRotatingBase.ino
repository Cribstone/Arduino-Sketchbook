#include <SoftwareSerial.h>

/*
* Switch and LED test program
*/

int ledPin = 5; // LED is connected to pin 12
int switchPin = 0; // switch is connected to pin 2
int val; // variable for reading the pin status
int ledPin6 = 6; // LED is connected to pin 12
int switchPin1 = 1; // switch is connected to pin 2
int val1; // variable for reading the pin status
void setup() {
pinMode(ledPin, OUTPUT); // Set the LED pin as output
pinMode(switchPin, INPUT); // Set the switch pin as input
pinMode(ledPin6, OUTPUT); // Set the LED pin as output
pinMode(switchPin1, INPUT); // Set the switch pin as input
//Setup Channel A
pinMode(12, OUTPUT); //Initiates Motor Channel A pin
pinMode(9, OUTPUT); //Initiates Brake Channel A pin

}
void loop(){

//delay(10);

//val = digitalRead(switchPin); // read input value and store it in val
//if (val == LOW) { // check if the button is pressed
//digitalWrite(ledPin, HIGH); // turn LED on
//Serial.begin(9600); // set up Serial library at 9600 bps
//Serial.print(“sw0 on: “);Serial.println(digitalRead(switchPin));

//forward @ full speed
digitalWrite(12, HIGH); //Establishes forward direction of Channel A
digitalWrite(9, LOW); //Disengage the Brake for Channel A
analogWrite(3, 200); //Spins the motor on Channel A at full speed

delay(.1); //// ***** THIS SETS THE ROTATION SPEED .1 is good for slow spin, 10 or 100+ will rotate further

//delay(10);
// }
if (val == HIGH) { // check if the button is not pressed
digitalWrite(ledPin, LOW); // turn LED off
delay(10);

digitalWrite(9, HIGH); //Eengage the Brake for Channel A

//delay(1000);

}

// delay(10);

val = digitalRead(switchPin1); // read input value and store it in val
if (val == LOW) { // check if the button is pressed
digitalWrite(ledPin6, HIGH); // turn LED on
//Serial.print(“on: “);Serial.println(digitalRead(switchPin1));

//backward @ half speed
digitalWrite(12, LOW); //Establishes backward direction of Channel A
digitalWrite(9, LOW); //Disengage the Brake for Channel A
analogWrite(3, 255); //Spins the motor on Channel A at half speed

delay(3000);

delay(10);
}
if (val == HIGH) { // check if the button is not pressed
digitalWrite(ledPin6, LOW); // turn LED off
//Serial.print(“1=off: “);Serial.println(digitalRead(switchPin1));

digitalWrite(9, HIGH); //Eengage the Brake for Channel A

delay(1000);

delay(10);
}

}
