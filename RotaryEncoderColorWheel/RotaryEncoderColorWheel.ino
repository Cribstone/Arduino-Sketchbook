//http://codeshield.diyode.com/code/rotary-rgbled/
#include <Servo.h> 

/* Rotary encoder read example */
#define ENCODER_A 14
#define ENCODER_B 15
#define ENCODER_PORT PINC
#define SWITCH 13
#define RGB_RED 11
#define RGB_GREEN 10
#define RGB_BLUE 9
#define LED 6


static uint8_t counter = 1;      //this variable will be changed by encoder input
int lastRotValue;

void setup()
{
    /* Setup encoder pins as inputs */
    pinMode(ENCODER_A, INPUT);
    digitalWrite(ENCODER_A, HIGH);
    pinMode(ENCODER_B, INPUT);
    digitalWrite(ENCODER_B, HIGH);
    pinMode(RGB_RED, OUTPUT);
    pinMode(RGB_BLUE, OUTPUT);
    pinMode(RGB_GREEN, OUTPUT);
}
 
void loop()
{
    int8_t tmpdata;
    tmpdata = read_encoder();
    if (tmpdata) {
        counter += tmpdata;
        Serial.println(counter);
    }
    int rotValue = ((counter / 4) % 7) + 1;
    if (rotValue != lastRotValue) {
        lastRotValue = rotValue;
        Serial.println(rotValue);
        setColour(rotValue);
    }
}
 
/* returns change in encoder state (-1,0,1) */
int8_t read_encoder()
{
    static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8_t old_AB = 0;
    old_AB <<= 2;                   //remember previous state
    old_AB |= ( ENCODER_PORT & 0x03 );  //add current state
    return ( enc_states[( old_AB & 0x0f )]);
}

void setColour(int col)
{
  switch (col) {
    case 1: //RED
      digitalWrite(RGB_RED, HIGH);
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, LOW);
      break;
    case 2: //ORANGE
      digitalWrite(RGB_RED, HIGH);
      analogWrite(RGB_GREEN, 128);
      digitalWrite(RGB_BLUE, LOW);
      break;
    case 3: //YELLOW
      digitalWrite(RGB_RED, HIGH);
      digitalWrite(RGB_GREEN, HIGH);
      digitalWrite(RGB_BLUE, LOW);
      break;
    case 4: //GREEN
      digitalWrite(RGB_RED, LOW);
      digitalWrite(RGB_GREEN, HIGH);
      digitalWrite(RGB_BLUE, LOW);
      break;
    case 5: //BLUE
      digitalWrite(RGB_RED, LOW);
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, HIGH);
      break;
    case 6: //INDIGO
      analogWrite(RGB_RED, 128);
      analogWrite(RGB_GREEN, 128);
      digitalWrite(RGB_BLUE, HIGH);
      break;
    case 7: //VIOLET
      analogWrite(RGB_RED, 180);
      digitalWrite(RGB_GREEN, LOW);
      digitalWrite(RGB_BLUE, HIGH);
      break;
  }
}  
  
