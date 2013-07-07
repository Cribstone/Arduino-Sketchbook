#include <Servo.h>

Servo pan, tilt;

void setup() {
  pan.attach( A5 );
  tilt.attach( A0 );
 
  Serial.begin( 9600 );    // Open the serial port.
}

void loop() {
  if( Serial.available() ) {
    byte b = Serial.read();
   
    // Map high 4 bits of incoming byte to pan rotation in degrees.
    pan.write( map( b >> 4, 0, 15, 0, 180 ) );
    delay( 15 );
   
    // Map low 4 bits of incoming byte to tilt rotation in degrees.
    tilt.write( map( b & 0x0F, 0, 15, 0, 180 ) );
    delay( 15 );
  }
}

