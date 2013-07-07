/*
* wiichuck.cpp
* Arduino library to interface with wii Nunchuck
*/
#include "Arduino.h"           // Arduino defines

#include "Wire.h"              // Wire (I2C) defines
#include "wiichuck.h"          // Defines for this library

// defines for standard Arduino board (use 19 and 18 for mega)
const int vccPin = 17;        // +v and gnd provided through these pins
const int gndPin = 16;


const int dataLength =            // number of bytes to request
static byte rawData[dataLength];  // array to store nunchuck data


// uses pins adjacent to I2C pins as power & ground for Nunchuck
void nunchuckSetPowerpins()
{
  pinMode(gndPin, OUTPUT); // set power pins to the correct state
  pinMode(vccPin, OUTPUT);
  digitalWrite(gndPin, LOW);
  digitalWrite(vccPin, HIGH);
  delay(100); // wait for power to stabilize
}

 // initialize the I2C interface for the nunchuck
 void nunchuckInit()
 {
  Wire.begin();
  Wire.beginTransmission(0x52);
  Wire.write((byte)0x40);
  Wire.write((byte)0x00);
  Wire.endTransmission();
}
void nunchuckRequest()
{
  Wire.beginTransmission(0x52);// transmit to device 0x52
  Wire.write((byte)0x00);// sends one byte
  Wire.endTransmission();// stop transmitting
}

 // Receive data back from the nunchuck,
 // returns true if read successful, else false
 bool nunchuckRead()
{
  byte cnt=0;
  Wire.requestFrom (0x52, dataLength);// request data from nunchuck
  while (Wire.available ()) {
    byte x = Wire.read();
    rawData[cnt] = nunchuckDecode(x);
    cnt++;
  }
  nunchuckRequest(); // send request for next data payload
  if (cnt >= dataLength)
   return true;
  else
   return false; 
}

// Encode data to format that most wiimote drivers except
char nunchuckDecode (byte x)
{
   return (x ^ 0x17) + 0x17;
}

// return the value for the given item
int nunchuckGetValue(int item)
{
  if( item <= wii_accelZ)
    return (int)rawData[item];
  else if(item == wii_btnZ)
    return bitRead(rawData[5], 0) ? 0: 1;
  else if(item == wii_btnC)
    return bitRead(rawData[5], 1) ? 0: 1;
}
