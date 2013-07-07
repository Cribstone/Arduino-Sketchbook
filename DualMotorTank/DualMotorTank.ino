#define A_direction 12
#define B_direction 13
#define A_speed 3
#define B_speed 11
#define A_brake 9
#define B_brake 8
int speed = 150;

void setup() 
{
  pinMode(A_direction,OUTPUT);
  pinMode(B_direction,OUTPUT);
  pinMode(A_speed,OUTPUT);
  pinMode(B_speed,OUTPUT);
  pinMode(A_brake,OUTPUT);
  pinMode(B_brake,OUTPUT);
  Serial.begin(9600);
  Serial.println("Use numpad keys to operate the tank");
  Serial.println("Use numpad key 0 to stop the tank");
}

void loop()
{
  if (Serial.available() > 0)
  {
    
    int data = Serial.read();
    switch (data)
    {
      case '8' :
      digitalWrite(A_direction,HIGH);
      digitalWrite(B_direction,HIGH);
      digitalWrite(A_brake,LOW);
      digitalWrite(B_brake,LOW);
      analogWrite(A_speed,speed);
      analogWrite(B_speed,speed);
      Serial.println("N"); 
      break;
      
      case '2' :
      digitalWrite(A_direction,LOW);
      digitalWrite(B_direction,LOW);
      digitalWrite(A_brake,LOW);
      digitalWrite(B_brake,LOW);
      analogWrite(A_speed,speed);
      analogWrite(B_speed,speed);
      Serial.println("S");
      break;
      
      case '4' :
      digitalWrite(A_direction,LOW);
      digitalWrite(B_direction,HIGH);
      digitalWrite(A_brake,LOW);
      digitalWrite(B_brake,LOW);
      analogWrite(A_speed,speed);
      analogWrite(B_speed,speed);
      Serial.println("W");
      break;
      
      case '6' :
      digitalWrite(A_direction,HIGH);
      digitalWrite(B_direction,LOW);
      digitalWrite(A_brake,LOW);
      digitalWrite(B_brake,LOW);
      analogWrite(A_speed,speed);
      analogWrite(B_speed,speed);
      Serial.println("E");
      break;
      
      case '0' :
      digitalWrite(A_direction,LOW);
      digitalWrite(B_direction,LOW);
      digitalWrite(A_brake,HIGH);
      digitalWrite(B_brake,HIGH);
      analogWrite(A_speed,0);
      analogWrite(B_speed,0);
      Serial.println("Stop!");
      break;
      
      default:
      digitalWrite(A_direction,LOW);
      digitalWrite(B_direction,LOW);
      digitalWrite(A_brake,HIGH);
      digitalWrite(B_brake,HIGH);
      analogWrite(A_speed,0);
      analogWrite(B_speed,0);
      break;      
  }
 }
}

