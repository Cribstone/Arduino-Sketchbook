#include <Wire.h>

int delaylength= 40;
int i = 0;
int UD = 0;

void setup() 
{
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT); 
  pinMode(8, OUTPUT); 
}

void loop()
{
  UD = analogRead(0) - 28;
  if (UD > 0)
  {
    digitalWrite(9, LOW);  
    digitalWrite(8, HIGH);
    digitalWrite(12, HIGH);   
    analogWrite(3, 1023);  
    delay(delaylength / abs(UD));
    digitalWrite(9, HIGH); 
    digitalWrite(8, LOW); 
    digitalWrite(13, LOW);   
    analogWrite(11, 1023);   
    delay(delaylength / abs(UD));
    digitalWrite(9, LOW);  
    digitalWrite(8, HIGH);
    digitalWrite(12, LOW);  
    analogWrite(3, 1023);   
    delay(delaylength / abs(UD));
    digitalWrite(9, HIGH); 
    digitalWrite(8, LOW); 
    digitalWrite(13, HIGH);  
    analogWrite(11, 1023);  
    delay(delaylength / abs(UD));
  }
  if (UD < 0)
  {
    digitalWrite(9, LOW);  
    digitalWrite(8, HIGH);
    digitalWrite(12, HIGH);   
    analogWrite(3, 1023);  
    delay(delaylength / abs(UD));
    digitalWrite(9, HIGH);  
    digitalWrite(8, LOW); 
    digitalWrite(13, HIGH);   
    analogWrite(11, 1023);   
    delay(delaylength / abs(UD));
    digitalWrite(9, LOW);  
    digitalWrite(8, HIGH); 
    digitalWrite(12, LOW);  
    analogWrite(3, 1023);   
    delay(delaylength / abs(UD));
    digitalWrite(9, HIGH);  
    digitalWrite(8, LOW); 
    digitalWrite(13, LOW);  
    analogWrite(11, 1023);   
    delay(delaylength / abs(UD));
  }
}
