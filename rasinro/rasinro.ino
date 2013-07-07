

#include <Servo.h> 

#define trigPin1 1
#define echoPin1 0

#define trigPin2 A3
#define echoPin2 A2

#define trigPin3 A5
#define echoPin3 A4



Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

int pos = 90;    // variable to store the servo position 
int turn_on =0;
String direction = "forward";
String direction2 = "forward";

char msgs[5][15] = {
  "Up Key OK    ",
  "Left Key OK  ",
  "Down Key OK  ",
  "Right Key OK ",
  "Stop Key OK" };
char start_msg[15] = {
  "Start loop "};                   
int  adc_key_val[5] ={
  30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

boolean light_a_on = false;
boolean light_b_on = false;

//Standard PWM DC control
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control

///For previous Romeo, please use these pins.
//int E1 = 6;     //M1 Speed Control
//int E2 = 9;     //M2 Speed Control
//int M1 = 7;    //M1 Direction Control
//int M2 = 8;    //M1 Direction Control

//int led1 = 10;
int led2 = 12;
int led3 = 11;
int led4 = 3;

void light1(int on){
  //digitalWrite(13, HIGH);
  //delay(20);
  if(on==0){
    //digitalWrite(led1, LOW);
    //analogWrite(led1, 100);

    //Serial.println("led 1 off");
    light_a_on = false;
  }else{
    //digitalWrite(led1, HIGH);

    //analogWrite(led1, 255);
    //Serial.println("led 1 on");
    light_a_on = true;
  }  
  //digitalWrite(13, LOW);
}

void light2(int on){
  //digitalWrite(13, HIGH);
  //delay(20);
  if(on==0){
    digitalWrite(led2, LOW);
    //analogWrite(led2, 100);
    Serial.println("Body light OFF");
    light_a_on = false;
    buzz(melodyPin, tones[7], 50);
  }else{
    digitalWrite(led2, HIGH);
    //analogWrite(led2, 255);
    Serial.println("Body light ON");
    light_a_on = true;
    buzz(melodyPin, tones[5], 50);
  }
  //digitalWrite(13, LOW);
}
void light3(int on){
  //digitalWrite(13, HIGH);
  //delay(20);
  if(on==0){
    digitalWrite(led3, LOW);
    //Serial.println("led 3 off");

  }else{
    digitalWrite(led3, HIGH);

    //Serial.println("led 3 on");

  }  
  //digitalWrite(13, LOW);
}

void light4(int on){
  //digitalWrite(13, HIGH);
  //delay(20);
  if(on==0){
    digitalWrite(led4, LOW);
    Serial.println("Head light OFF");
    buzz(melodyPin, tones[1], 50);
    light_b_on = false;
  }else{

    digitalWrite(led4, HIGH);
    Serial.println("Head light ON");
    buzz(melodyPin, tones[3], 50);
    light_b_on = true;
  }
  //digitalWrite(13, LOW);
}

void sing(int song)          //Move forward
{
  Serial.println("Singing:");
  if(song==0)
    song=song_counter;

  if(song_counter>4)
    song_counter=1;
  else  
    song_counter++;
  if(song==1){
    Serial.println("'Do re mi'");
    int size = sizeof(tones) / sizeof(int);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    for (int thisNote = 0; thisNote < size; thisNote++) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 30;

        int d = thisNote*15;
        analogWrite(led3, d);
        analogWrite(led4, d);
        buzz(melodyPin, tones[thisNote],noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 0.30;
        delay(pauseBetweenNotes);

        // stop the tone playing:
        buzz(melodyPin, 0,noteDuration);
      }
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH);
    }else if(song==2){
    Serial.println("'Do si la'");
    int size = sizeof(tones) / sizeof(int);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    for (int thisNote = size; thisNote >=0 ; thisNote--) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       int noteDuration = 30;
       int d = thisNote*15;
       analogWrite(led3, d);
       analogWrite(led4, d);
       buzz(melodyPin, tones[thisNote],noteDuration);

       // to distinguish the notes, set a minimum time between them.
       // the note's duration + 30% seems to work well:
       int pauseBetweenNotes = noteDuration * 0.30;
       delay(pauseBetweenNotes);

       // stop the tone playing:
        buzz(melodyPin, 0,noteDuration);
      }
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
    }else if(song==3){
      Serial.println("'Tetot'");
      int size = sizeof(melody) / sizeof(int);
      for (int thisNote = 0; thisNote < size; thisNote++) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       int noteDuration = 1000/tempo[thisNote];

        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH);
       buzz(melodyPin, melody[thisNote],noteDuration);
       digitalWrite(led3, LOW);
       digitalWrite(led4, LOW);

       // to distinguish the notes, set a minimum time between them.
       // the note's duration + 30% seems to work well:
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);

       // stop the tone playing:
      buzz(melodyPin, 0,noteDuration);

    }

  }else if(song==4){

     Serial.println("'Mario Theme'");
     int size = sizeof(mario_melody) / sizeof(int);
     for (int thisNote = 0; thisNote < size; thisNote++) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       int noteDuration = 1000/mario_tempo[thisNote];
        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH);
       buzz(melodyPin, mario_melody[thisNote],noteDuration);
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW);
       // to distinguish the notes, set a minimum time between them.
       // the note's duration + 30% seems to work well:
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);

       // stop the tone playing:
       buzz(melodyPin, 0,noteDuration);

    }

   }else{
     Serial.println("'Underworld Theme'");
     int size = sizeof(underworld_melody) / sizeof(int);
     for (int thisNote = 0; thisNote < size; thisNote++) {

       // to calculate the note duration, take one second
       // divided by the note type.
       //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
       int noteDuration = 1000/underworld_tempo[thisNote];

        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH);
       buzz(melodyPin, underworld_melody[thisNote],noteDuration);
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW);
       // to distinguish the notes, set a minimum time between them.
       // the note's duration + 30% seems to work well:
       int pauseBetweenNotes = noteDuration * 1.30;
       delay(pauseBetweenNotes);

       // stop the tone playing:
       buzz(melodyPin, 0,noteDuration);
     }
  }
  if(light_a_on){ //keep the light on after blinking
       light1(1);
       light2(1);
     }else{
       light1(0);
       light2(0);
     } 
  if(light_b_on){ //keep the light on after blinking
       light3(1);
       light4(1);
     }else{
       light3(0);
       light4(0);
     }   
} 

void stop(void)                    //Stop
{
  digitalWrite(E1,LOW);  
  digitalWrite(E2,LOW);     
  //digitalWrite(13, HIGH);
  //delay(10);
  //digitalWrite(13, LOW);
  buzz(melodyPin, tones[11], 100);
  delay(10);
  buzz(melodyPin, tones[11], 100);
  direction = "stop";
}  
void advance(int a,int b)          //Move forward
{ 
    buzz(melodyPin, tones[0], 100);
    buzz(melodyPin, tones[4], 100);
    int x=0;
    for(x = 50; x<=a;x++){
      analogWrite (E1,x);      //PWM Speed Control
      digitalWrite(M1,HIGH);       
      analogWrite (E2,x); 
      digitalWrite(M2,HIGH);

      //Serial.print(x);  
      delay(10);
    }
    direction = "forward";

} 
void back_off (int a,int b)          //Move backward
{
  if(checkDistance(trigPin2, echoPin2)>=23){ //give a bit of lee way than the 20cm
    buzz(melodyPin, tones[4], 100);
    buzz(melodyPin, tones[0], 100);

    for(int x = 50; x<=a;x++){
      analogWrite (E1,x);
      digitalWrite(M1,LOW);  
      analogWrite (E2,x);   
      digitalWrite(M2,LOW);
      delay(10);
    }
    direction="reverse";
    direction2="center";
  }else{
     emergencyStop();

  }
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (E1,a);
  digitalWrite(M1,LOW);   
  analogWrite (E2,b);   
  digitalWrite(M2,HIGH);
  buzz(melodyPin, tones[3], 100);
  buzz(melodyPin, tones[5], 100);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);   
  analogWrite (E2,b);   
  digitalWrite(M2,LOW);
  buzz(melodyPin, tones[4], 100);
  buzz(melodyPin, tones[6], 100);

}
void turn_FL (char a,char b)             //Forward Left
{

  analogWrite (E1,a);
  digitalWrite(M1,HIGH);   
  analogWrite (E2,b);   
  digitalWrite(M2,HIGH);
  direction = "forward";
  direction2="left";
  buzz(melodyPin, tones[5], 100);
  buzz(melodyPin, tones[7], 100);
}
void turn_FR (char a,char b)             //Forward Right
{

  analogWrite (E1,a);
  digitalWrite(M1,HIGH);   
  analogWrite (E2,b);   
  digitalWrite(M2,HIGH);
  direction = "forward";

  buzz(melodyPin, tones[6], 100);
  buzz(melodyPin, tones[8], 100);
}
void turn_RL (char a,char b)             //Reverse Left
{
  if(checkDistance(trigPin1, echoPin1)>=15){
    analogWrite (E1,a);
    digitalWrite(M1,LOW);   
    analogWrite (E2,b);   
    digitalWrite(M2,LOW);
    direction = "reverse";
    direction2="left";
    buzz(melodyPin, tones[7], 50);
    buzz(melodyPin, tones[9], 50);
  }else{
    emergencyStop();
  }

}
void turn_RR (char a,char b)             //Reverse Right
{
  if(checkDistance(trigPin3, echoPin3)>=15){
    analogWrite (E1,a);
    digitalWrite(M1,LOW);   
    analogWrite (E2,b);   
    digitalWrite(M2,LOW);
    direction = "reverse";
    direction2="right";
    buzz(melodyPin, tones[10], 50);
    buzz(melodyPin, tones[8], 50);
  }else{
    emergencyStop();

  }
}

void setup(void)
{
  int i;
  //pinMode(10, OUTPUT);  //we'll use the debug LED to output a heartbeat

  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT); 

  myservo.attach(A1);
  myservo.write(90);
  int z=0;
  int a=0;

  pinMode(2, OUTPUT);

  pinMode(2, OUTPUT);

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  Serial.begin(19200);      //Set Baud Rate
  Serial.println("Run keyboard control");
  sing(1);
  sing(2);
} 

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

int distance_status=0; 
int s=0;

void loop()
{

  //delay(1000); // wait a bit between buzzes

    //analogWrite(10, brightness);
//for(int d=0;d<=13;d++)
  //analogWrite(d,brightness);
//analogWrite(12, brightness;
/*

analogWrite(11, brightness);    
analogWrite(13, brightness);    

  // change the brightness for next time through the loop:

  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }    
  // wait for 30 milliseconds to see the dimming effect    
  delay(30); 
*/
  adc_key_in = analogRead(A0);    // read the value from the sensor 

  /* get the key */
  key = get_key(adc_key_in);    // convert into key press
  if (key != oldkey) {   // if keypress is detected
    delay(50);      // wait for debounce time
    adc_key_in = analogRead(A0);    // read the value from the sensor 
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey) {        
      oldkey = key;
      if (key >=0){
        Serial.println(adc_key_in, DEC);
        Serial.println(msgs[key]);
        Serial.println(key);
    }
  }

          int val2 =key;
    if(val2 != -1)
    {
      switch(val2)
      {
      case 0://Move Forward
        advance (200,200);   //move forward in half speed
        break;
      case 2://Move Backward
        back_off (175,175);   //move back in a bit slower
        break;
      case 1://Turn Left
        turn_L (255,255);
        break;      
      case 3://Turn Right
        turn_R (255,255);
        break;
      case 4:
        stop();
        break;
      }
    }
    else stop();   
      }

  if(Serial.available()){
    char val = Serial.read();

    if(val != -1)
    {
      switch(val)
      {

      case 'w'://Move Forward
        advance (255,255);   //move forward in half (max speed 255)
        Serial.println("Forward");
        break;
      case 'x'://Move Backward
        back_off (255,255);   //move back in slow spee
        Serial.println("Reverse");

        break;
      case 'a'://Turn Left
        turn_L (250,255);
        Serial.println("Left");

        break;      
      case 'd'://Turn Right
        turn_R (255,250);
        Serial.println("Right");

        break;
       case 'q'://Turn Left
        turn_FL (60,255);
        Serial.println("Forward Left");
        break;      
      case 'e'://Turn Right
         turn_FR (255,60);
        Serial.println("Forward Right");
        break;
       case 'z'://Turn Left
        turn_RL (60,255);
        Serial.println("Reverse Left");

        break;      
      case 'c'://Turn Right
        turn_RR (255,60);
        Serial.println("Reverse Right");

        break;
      case 'h':
        Serial.println("Hello");
        break;
      case 's':
        stop();
        Serial.println("Stop!");

        break;
      case 'y':
        sing(0);
        break;
      case '1':
         tilt(60);
          break;
        case '2':
          tilt(90);
          break;
        case '3':
          tilt(120);
          break;          
         case 'u':
          light1(1);
          light2(1);
          light_a_on=true;
          break;
         case 'i':
           light3(1);
           light4(1);
           light_b_on=true;
           break;
         case 'o':
          light1(0);
          light2(0);
          light_a_on=false;
          break;
         case 'p':
           light3(0);
           light4(0); 
           light_b_on=false;
           break;
         case 'l':
           Serial.print("lcd");
           break;

      }

    }
    else stop(); 
  }  

  s = distance_status %5; 

   if(direction=="reverse"){
     if(direction2=="left"){
       if(checkDistance(trigPin2, echoPin2)<25 || checkDistance(trigPin3, echoPin3)<20){
         Serial.println("Obstacles\non the left!");
         emergencyStop();
         distance_status=0;
       }
     }else if(direction2=="center"){
       if(checkDistance(trigPin2, echoPin2)<25 || checkDistance(trigPin3, echoPin3)<20 || checkDistance(trigPin1, echoPin1)<20){
         Serial.println("Obstacles\nat the rear!");
         emergencyStop();
         distance_status=0;
       }
     }else{
       if(checkDistance(trigPin2, echoPin2)<25 || checkDistance(trigPin1, echoPin1)<20){
         Serial.println("Obstacles\non the right!");
         emergencyStop();
         distance_status=0;
       }       

     }
   }else
   {
      distance_status=0; 
      s=0;
   }

}

void tilt(int t){
           myservo.write(t);
         buzz(melodyPin, tones[10], 100);
         buzz(melodyPin, tones[12], 100);
         buzz(melodyPin, tones[14], 100); 
}
void emergencyStop(){
  stop();
    Serial.println("Stopping");

     for(int s=0;s<2;s++){
       light1(1);
       light2(1);
       delay(50);
       light1(0);
       light2(0);
       delay(50);
     }
     if(light_a_on){ //keep the light on after blinking
       light1(1);
       light2(1);
     }else{
       light1(0);
       light2(0);
     } 
}
// Convert ADC value to key number
int get_key(unsigned int input)
{  
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input <= adc_key_val[k])
    { 
      return k; 
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed
  return k;
}

double checkDistance(int trig, int echo){
  long duration, distance;
  String sensor ="";
  if(trig ==1)
    sensor = "Right Sensor";
  else if(trig == A3)
    sensor = "Center Sensor";
  else
    sensor = "Left Sensor";
  digitalWrite(trig, LOW);  // Added this line
  delayMicroseconds(2); // Added this line

  digitalWrite(trig, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(100); // Added this line
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
  if (distance >= 200 || distance <= 10){
    Serial.println(trig + ": Out of range");
  }
  else {
    if(s==0){
      Serial.print(sensor);
      Serial.print(" : ");
      Serial.print(distance);
      Serial.println(" cm");
     }
    //output.print(distance );
    //output.println(" cm");
  }
  distance_status++;
  delay(50);
  return distance;

} 

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13,HIGH);
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
 for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait againf or the calculated delay value
  }
  digitalWrite(13,LOW);
}