/*

This example flashes the LED on pin 13

when receive from Bluetooth (serial)

The BT-Module is connected to Pin 0 and 1 like the serial interface

*/

int led = 13; // the pin that the LED is attached to

char chByte = 0; // incoming serial byte

String strInput = “”; // buffer for incoming packet

String strCompare = “switch”;

// the setup routine runs once when you press reset:

void setup() {

// declare pin 9 to be an output:

pinMode(led, OUTPUT);

// initialize serial:

Serial.begin(9600);

}

// the loop routine runs over and over again forever:

void loop()

{

while (Serial.available() > 0)

{

// get incoming byte:

chByte = Serial.read();

if (chByte == ‘\r’)

{

//compare input message

if(strInput.equals(strCompare))

{

//toggle LED

digitalWrite(led, HIGH);

Serial.println(“LED is ON”);

delay(1000);

digitalWrite(led, LOW);

Serial.println(“LED is OFF”);

}

//reset strInput

strInput = “”;

}

else

{

strInput += chByte;

}

}

}


