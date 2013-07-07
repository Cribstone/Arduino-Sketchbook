int val=0;

void setup()
{
Serial.begin(9600);  
pinMode(13,OUTPUT);
digitalWrite(13,LOW);
}

void loop()
{
digitalWrite(13,HIGH);
delay(250);
int val = analogRead(A0);
float voltage = (val) * 3.3;
voltage /= 1024.0;
Serial.print(voltage); Serial.println(" volts");
float temperatureC = (voltage - 0.5) * 10 ; 
Serial.print(temperatureC);
Serial.println(" degrees C");
digitalWrite(13,LOW);
delay(250);
}


