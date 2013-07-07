//Arduino PWM Speed Control for DFRobot Motor Shield
//
 
int E1 = 6;
int M1 = 7;
int E2 = 5;
int M2 = 4;
 
void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}
 
void loop()
{
  int value;
  for(value = 0 ; value <= 255; value+=5)
  {
    digitalWrite(M1,HIGH);
    digitalWrite(M2, HIGH);
    analogWrite(E1, value);   //PWM Speed Control
    analogWrite(E2, value);   //PWM Speed Control
    delay(30);
  }
}

