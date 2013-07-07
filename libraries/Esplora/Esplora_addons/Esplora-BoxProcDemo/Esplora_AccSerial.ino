/* 
   Created Dec 31, 2012
   by w9jds (Jeremy Shore)

   This takes the information from the accelerometer on the Esplora
   and transfers the values over serial. It only reads and sends while 
   read is toggled to true (green light is on). How high the values of
   the accelerometer are based on the potentiometer.
*/

#include <Esplora.h>

boolean PrevState;
boolean Read = false;

void setup()
{
  Serial.begin(9600);
} 

void loop()
{
  boolean newState = Esplora.readButton(SWITCH_3);
  if (newState != PrevState){
    if (newState == PRESSED)
      Read = !Read;
    PrevState = newState;
  }
  
  if (Read == true){
    Esplora.writeGreen(10);
    int threshold = map(Esplora.readSlider(), 0, 1024, 200, 20);
    int x = map(Esplora.readAccelerometer(X_AXIS), -1024, 1024, -threshold, threshold);
    int y = map(Esplora.readAccelerometer(Y_AXIS), -1024, 1024, -threshold, threshold);
    int z = map(Esplora.readAccelerometer(Z_AXIS), -1024, 1024, -threshold, threshold);
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.print(',');
    Serial.print(z);
    Serial.print(';');
  }
  else if (Read == false)
    Esplora.writeGreen(0);

  delay(60);
}

