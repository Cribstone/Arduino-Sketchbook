/*
Connects to a Bluetooth module and parses data sent from a device. Works with Arduino Uno and also tested to work 
with a Digispark (must remove all Serial.print)
Expects string data to come as follows:
sX=[xVal],Y=[yVal]e  - used for sending X/Y data. Example:  sX=75,Y=55e.  Handles +/-
sC=[cVal]e - used for sending C (command button) data. Example: sC=1e
*/


#include <SoftwareSerial.h>
//for Digispark
//#define RxPin 3  //goes to TX pin on BT module
//#define TxPin 4 //goes to RX pin on BT module

//for testing on UNO
#define RxPin 0  //goes to TX pin on BT module
#define TxPin 1 //goes to RX pin on BT module

#define SOP 's'
#define EOP 'e'
#define leftMtrDirPin 8
#define leftMtrSpdPin 6
#define rightMtrDirPin 7
#define rightMtrSpdPin 5
char inData[20];
byte index;
int xVal=0, xAdj=0, yVal=0, yAdj=0, cVal=0;
int leftSpeed=0, rightSpeed=0;
int leftDir = HIGH, rightDir = HIGH;
bool started = false;
bool ended = false;
SoftwareSerial blueToothSerial(RxPin, TxPin); // RX, TX

void setup()
{
   Serial.begin(9600);
   blueToothSerial.begin(9600);
   // Other stuff...
   pinMode(RxPin, INPUT);
   pinMode(TxPin, OUTPUT);
   pinMode(leftMtrSpdPin, OUTPUT);   
   pinMode(leftMtrDirPin, OUTPUT);
   pinMode(rightMtrDirPin, OUTPUT);
   pinMode(rightMtrSpdPin, OUTPUT);  
}

void loop()
{
  xVal=0;
  yVal=0;
  cVal=0;

  // Read all serial data available, as fast as possible
  //while(Serial.available() > 0)
  
      if (blueToothSerial.available() > 0){
        char choice = blueToothSerial.read();
         Serial.print("Val="); 
         Serial.println(choice); 
         
              switch(choice){
                  case 'U':
                      driveMotors(0, 100);  //forward
                      break;
                  case 'D':
                      driveMotors(0, -100); //backward           
                      break;
                  case 'L':
                      driveMotors(-100, 0);  //left
                      break;
                  case 'R':
                      driveMotors(100, 0);   //right
                      break;
                  case 'S':
                      driveMotors(0, 0);   //stop - or could use for other function. 
                      break;
                  case '1':
                    cVal = 1;  //command value 1
                    //do something or use cVal to do something later.
                      break;
                  case '2':
                    cVal = 2;
                      break;
                  case '3':
                    cVal = 3;
                      break;
                  case '4':
                    cVal = 4;
                      break;
                  case '5':
                    cVal = 5;
                      break;
                  case '6':
                    cVal = 6;
                      break;
                  case '7':
                    cVal = 7;
                      break;
                  case '8':
                    cVal = 8;
                      break;
              } 
         
      }  //end bluetoothSerial.available

     
     // you can add your own code here to process and react to cVal values.  You can also process U,D,L,R values
     // here to drive the motors.

  
}  //end loop


void driveMotors(int xVal, int yVal)
{ 
  //both motors forward full speed
  //digitalWrite(MotorADir, HIGH);   //forward
  //digitalWrite(MotorBDir, LOW); 
  //analogWrite(MotorASpeed, 100); //100 will be the lowest speed to start at.
  //analogWrite(MotorBSpeed, 100);
  
  float xPct=1.0;
  
  xAdj = map(abs(xVal), 0, 100, 50, 255);  //start motor speed of 50 is as low as these motors will work.  Your motors may be different
  yAdj = map(abs(yVal), 0, 100, 50, 255);  //start motor speed of 50 is as low as these motors will work.  Your motors may be different   
   
   
  //xPct = map(abs(xAdj), 0, 255, 1, 0);  
  xPct = mapf((float)abs(xAdj), (float)0, (float)255, (float)1, (float)0);  
   
   
 //Serial.print("  xadj: "); printDouble(xAdj,10);
 //Serial.print("  yadj: "); printDouble(yAdj,10);
 //Serial.print("  yVal: "); printDouble(yVal,10);
 //Serial.print("  xAdj: "); printDouble(xAdj,10);
 //Serial.print("  xVal: "); printDouble(xVal,10);
 //Serial.print("  x%: "); printDouble(xPct,10);   
 //drive based on quadrant

  
  if (((xVal > -20) && (xVal < 20)) && (((yVal <= 100) && (yVal > 60)) || ((yVal >= -100) && (yVal < -60))))
  {
    if  ((yVal <= 100) && (yVal > 60))    //1:  joystick full fwd.  Both motors full speed ahead.
    {
      digitalWrite(leftMtrDirPin, HIGH);  
      digitalWrite(rightMtrDirPin, LOW); //yes - messed up soldering motor leads - LOW makes FWD in this case.
      analogWrite(leftMtrSpdPin, 255);   //drive the motor
      analogWrite(rightMtrSpdPin, 255);  //drive the motor
      //Serial.print(" - Full FWD");   
    }
    else if ((yVal >= -100) && (yVal < -60))  //2:  joystick full rev. Both motors full speed rev.
    {
      digitalWrite(leftMtrDirPin, LOW);  
      digitalWrite(rightMtrDirPin, HIGH); //yes - messed up soldering motor leads - HIGH makes REV in this case.
      analogWrite(leftMtrSpdPin, 255);   //drive the motor
      analogWrite(rightMtrSpdPin, 255);  //drive the motor     
      //Serial.print(" - Full REV");   
    }
    
  }
  else if  (((yVal > -20) && (yVal < 20)) && (((xVal <= 100) && (xVal > 60)) || ((xVal >= -100) && (xVal < -60))))
  {
    if ((xVal <= 100) && (xVal > 60))  //3:  Fast turn right. Rt Mtr full fwd. Left Mtr full rev.
    {
      digitalWrite(leftMtrDirPin, HIGH);  
      digitalWrite(rightMtrDirPin, HIGH); 
      analogWrite(leftMtrSpdPin, 150);   //drive the motor
      analogWrite(rightMtrSpdPin, 150);  //drive the motor
      //Serial.print(" - Full RT");   
    }
    else if ((xVal >= -100) && (xVal < -60))  //4: Fast turn left: Left Mtr full fwd. Rt Mtr full rev.
    {
      digitalWrite(leftMtrDirPin, LOW);  
      digitalWrite(rightMtrDirPin, LOW); 
      analogWrite(leftMtrSpdPin, 150);   //drive the motor
      analogWrite(rightMtrSpdPin, 150);  //drive the motor
      //Serial.print(" - Full LF");   
    }
  }
   
  // if not extreme corners, do normal movements. 
  else if (yVal > 0)  //going forward
  {
    //robot driving fwd - now determine left and right.
    digitalWrite(leftMtrDirPin, HIGH);  
    digitalWrite(rightMtrDirPin, LOW); 
    
    if (xVal < -15) //going left  - leave room for going straight
    {
      analogWrite(leftMtrSpdPin, yAdj * xPct);    //drive the motor
      analogWrite(rightMtrSpdPin, yAdj);  //drive the motor
      //Serial.print("FwLF: "); printDouble(xPct,10);
    }
    else if (xVal > 15)
    {  //going right
      analogWrite(leftMtrSpdPin, yAdj);    //drive the motor
      analogWrite(rightMtrSpdPin, yAdj  * xPct);  //drive the motor    
      //Serial.print("FwRT: "); printDouble(xPct,10);
    }
    else  //x is beteen -15 and 15 - assume going straight
    {
      analogWrite(leftMtrSpdPin, yAdj);    //drive the motor
      analogWrite(rightMtrSpdPin, yAdj);   //drive the motor    
      //Serial.print("Normal FWD: ");
    }
  }
  else   //y val is < 0 - reversing
  {
    //robot driving backwards - now determine left and right.
    digitalWrite(leftMtrDirPin, LOW);  
    digitalWrite(rightMtrDirPin, HIGH); 

    if (xVal < -15) //going left  - leave room for going straight
    {
      analogWrite(leftMtrSpdPin, yAdj * xPct);    //drive the motor
      analogWrite(rightMtrSpdPin, yAdj);  //drive the motor
      //Serial.print("RevLF: "); printDouble(xPct,10);
    }
    else if (xVal > 15)
    {
      analogWrite(leftMtrSpdPin, yAdj);    //drive the motor
      analogWrite(rightMtrSpdPin, yAdj  * xPct);  //drive the motor    
      //Serial.print("RevRT: "); printDouble(xPct,10);
    }
    else  //x is beteen -15 and 15 - assume going straight
    {
      analogWrite(leftMtrSpdPin, yAdj);    //drive the motor
      analogWrite(rightMtrSpdPin, yAdj);   //drive the motor    
      //Serial.print("NormalRev: "); 
    }

    

  }
   //slight delay
  delay(50);
  //Serial.println();
  leftSpeed=0, rightSpeed=0, xAdj=0, yAdj=0;
}


void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    Serial.println(frac,DEC);
}   
 
//use to map values to float.   
float mapf (float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
