#include <Wire.h>



uint8_t outbuf[6];

int cnt = 0;
int ledPin = 13;

int servoPin = 9;
int servoPin2 = 10;

int pulseWidth = 0;
int pulseWidth2 = 0;

long lastPulse = 0;
long lastPulse2 = 0;

int z_button = 0;
int c_button = 0;

int refreshTime = 20;

int minPulse = 1000;
int minPulse2 = 500;

int dtime=10;

#define pwbuffsize 10
long pwbuff[pwbuffsize];
long pwbuffpos = 0;
long pwbuff2[pwbuffsize];
long pwbuffpos2 = 0;

void setup()
{
  Serial.begin (9600);
  Wire.begin ();
  nunchuck_init ();
  pinMode(servoPin, OUTPUT);
  pinMode(servoPin2, OUTPUT);

  pulseWidth = minPulse;
  pulseWidth2 = minPulse2;
  Serial.print ("Finished setupn");
}

void nunchuck_init()
{
  Wire.beginTransmission (0x52);
  Wire.write (0x40);
  Wire.write (0x00);
  Wire.endTransmission ();
}

void write_zero()
{
  Wire.beginTransmission (0x52);
  Wire.write (0x00);
  Wire.endTransmission ();
}

int t = 0;

void loop()
{
  t++;
  long last = millis();

  if( t == 1) {

    t = 0;

    Wire.requestFrom (0x52, 6);

    while (Wire.available ()) {
      outbuf[cnt] = nunchuk_decode_byte (Wire.read ());
      digitalWrite (ledPin, HIGH);
      cnt++;
    }

    if (cnt >= 5) {

      //            printNunchuckData();

      int z_button = 0;
      int c_button = 0;

      if ((outbuf[5] >> 0) & 1)
        z_button = 1;
      if ((outbuf[5] >> 1) & 1)
        c_button = 1;

      switch (c_button) {
      case 1:
        switch (z_button) {
        case 0:
          break;
        case 1:
          muovi();
          break;
        }
        break;
      case 0:
        switch (z_button) {
        case 0:
          delay(10000);
          break;
        case 1:
          delay(3000);
          break;
        }
        break;
      }
    }

    cnt = 0;
    write_zero();

  } // if(t==)

  updateServo();

  delay(dtime);
}


void updateServo() {

  if (millis() - lastPulse >= refreshTime) {

    digitalWrite(servoPin, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(servoPin, LOW);

    digitalWrite(servoPin2, HIGH);
    delayMicroseconds(pulseWidth2);
    digitalWrite(servoPin2, LOW);

    lastPulse = millis();
  }
}

int i=0;
void printNunchuckData()
{
  int joy_x_axis = outbuf[0];
  int joy_y_axis = outbuf[1];
  int accel_x_axis = outbuf[2]; // * 2 * 2;
  int accel_y_axis = outbuf[3]; // * 2 * 2;
  int accel_z_axis = outbuf[4]; // * 2 * 2;

  int z_button = 0;
  int c_button = 0;

  if ((outbuf[5] >> 0) & 1)
    z_button = 1;
  if ((outbuf[5] >> 1) & 1)
    c_button = 1;
  if ((outbuf[5] >> 2) & 1)
    accel_x_axis += 2;
  if ((outbuf[5] >> 3) & 1)
    accel_x_axis += 1;

  if ((outbuf[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((outbuf[5] >> 5) & 1)
    accel_y_axis += 1;

  if ((outbuf[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((outbuf[5] >> 7) & 1)
    accel_z_axis += 1;

  Serial.print (i,DEC);
  Serial.print ("t");

  Serial.print ("X: ");
  Serial.print (joy_x_axis, DEC);
  Serial.print ("t");

  Serial.print ("Y: ");
  Serial.print (joy_y_axis, DEC);
  Serial.print ("t");

  Serial.print ("AccX: ");
  Serial.print (accel_x_axis, DEC);
  Serial.print ("t");

  Serial.print ("AccY: ");
  Serial.print (accel_y_axis, DEC);
  Serial.print ("t");

  Serial.print ("AccZ: ");
  Serial.print (accel_z_axis, DEC);
  Serial.print ("t");

  Serial.print (z_button, DEC);
  Serial.print (" ");
  Serial.print (c_button, DEC);
  Serial.print ("rn");
  i++;
}

char nunchuk_decode_byte (char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}

void muovi (){
  float tilt = (700 - outbuf[3]*2*2);
  float tilt2 = outbuf[2]*2*2;

  tilt = (tilt);
  pulseWidth = (tilt * 5) + minPulse;

  tilt2 = (tilt2-288);
  pulseWidth2 = (tilt2 * 5) + minPulse2;

  pwbuff[pwbuffpos] = pulseWidth;
  pwbuff2[pwbuffpos2] = pulseWidth2;

  if( ++pwbuffpos == pwbuffsize ) pwbuffpos = 0;
  if( ++pwbuffpos2 == pwbuffsize ) pwbuffpos2 = 0;


  pulseWidth=0;
  pulseWidth2=0;

  for( int p=0; p<pwbuffsize; p++ ){
    pulseWidth += pwbuff[p];
    pulseWidth2 += pwbuff2[p];
  }

  pulseWidth /= pwbuffsize;
  pulseWidth2 /= pwbuffsize;

}

