int rollDelay = 0;
int command = 0;
int rollSteps = 0;
int Step = 0;
int cycle = 0;
byte senseMotion = 0;

void setup() 
{
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(2, INPUT);
}

void loop()
{
  //Motion Sensor 
  senseMotion = digitalRead(2);
  if (senseMotion == HIGH) 
    {
       digitalWrite(9, LOW);
       digitalWrite(8, HIGH);
       digitalWrite(12, HIGH);
       analogWrite(3, 1023);
       delay(rollDelay);
       digitalWrite(9, HIGH);
       digitalWrite(8, LOW);
       digitalWrite(13, LOW);
       analogWrite(11, 1023);
       delay(rollDelay);
       digitalWrite(9, LOW);
       digitalWrite(8, HIGH);
       digitalWrite(12, LOW);
       analogWrite(3, 1023);
       delay(rollDelay);
       digitalWrite(9, HIGH);
       digitalWrite(8, LOW);
       digitalWrite(13, HIGH);
       analogWrite(11, 1023);
       delay(rollDelay);
     }
     if (command == 1)
     {
        digitalWrite(9, LOW);
        digitalWrite(8, HIGH);
        digitalWrite(12, HIGH);
        analogWrite(3, 255);
        delay(rollDelay);
        digitalWrite(9, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(13, HIGH);
        analogWrite(11, 255);
        delay(rollDelay);
        digitalWrite(9, LOW);
        digitalWrite(8, HIGH);
        digitalWrite(12, LOW);
        analogWrite(3, 255);
        delay(rollDelay);
        digitalWrite(9, HIGH);
        digitalWrite(8, LOW);
        digitalWrite(13, LOW);
        analogWrite(11, 255);
        delay(rollDelay);
      }
   }
