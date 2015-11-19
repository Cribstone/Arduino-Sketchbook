
/* Sweep Lidar Scanner
*  Lidar Lite sensor sending cm distance while servos sweep
* Dincer Hepguler 2015
* http://borsaci06.com
* Full Tutorial at http://myrobotlab.org/content/lidar-lite-distance-measurement-and-creating-point-cloud
*/
 
#include <i2cmaster.h>
#include <Servo.h> 
#include <I2C.h>
#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.
 
 
Servo myXservo;  // create servo object to control X servo 
Servo myYservo;  // create servo object to control Y servo
 
int posX = 0;    // variable to store X servo position 
int posY = 0;    // variable to store Y servo position 
 
void setup() 
{ 
  Serial.begin(57600);
  myXservo.attach(10);  // attaches X servo on pin 10 to the servo object 
  myYservo.attach(9);   // attaches Y servo on pin 9 to the servo object  
  I2c.begin();          // Opens & joins the irc bus as master
  delay(100);        // Waits to make sure everything is powered up before sending or receiving data  
  I2c.timeOut(50);    // Sets a timeout to ensure no locking up of sketch if I2C communication fails
} 
 
void loop() 
{ 
  while (millis() > 30000);{
  for(posX = 30; posX <= 150; posX += 3) // goes from 30 degrees to 150 degrees 
  {                                  
    for(posY = 60; posY <= 140; posY += 2){  //Y axis sweep
    myXservo.write(posX);              // tell servo to go to position in variable 'pos' 
    myYservo.write(posY);
    //delay(1);                       // waits for the servo to reach the position
  GetDist();
   } 
   for(posY = 140; posY >=60; posY -=2){ //Y axis sweep
    myXservo.write(posX);              // tell servo to go to position in variable 'pos' 
    myYservo.write(posY);
    //delay(1);                       // waits for the servo to reach the position 
  GetDist();
   }
  }   
  for(posX = 150; posX >=30; posX -=3)     // goes from 150 degrees to 30 degrees 
  { 
    for(posY = 60; posY <= 140; posY += 2){     //Y axis sweep
    myXservo.write(posX);              // tell servo to go to position in variable 'pos' 
    myYservo.write(posY);
    //delay(1);                       // waits for the servo to reach the position
  GetDist();
   }     
    for(posY = 140; posY >=60; posY -=2){     //Y axis sweep
    myXservo.write(posX);              // tell servo to go to position in variable 'pos' 
    myYservo.write(posY);
    //delay(1);                       // waits for the servo to reach the position 
  GetDist();
   } 
  }  
}   
    myXservo.write(90);
    myYservo.write(90);
    delay(25);
}
 
void GetDist(){
// Write 0x04 to register 0x00
  uint8_t nackack = 100; // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.write(LIDARLite_ADDRESS,RegisterMeasure, MeasureValue); // Write 0x04 to 0x00
    delay(1); // Wait 1 ms to prevent overpolling
  }
  byte distanceArray[2]; // array to store distance bytes from read function
  // Read 2byte distance from register 0x8f
  nackack = 100;        // Setup variable to hold ACK/NACK resopnses     
  while (nackack != 0){ // While NACK keep going (i.e. continue polling until sucess message (ACK) is received )
    nackack = I2c.read(LIDARLite_ADDRESS,RegisterHighLowB, 2, distanceArray); // Read 2 Bytes from LIDAR-Lite Address and store in array
    delay(1); // Wait 1 ms to prevent overpolling
  }
  int distance = (distanceArray[0] << 8) + distanceArray[1];  // Shift high byte [0] 8 to the left and add low byte [1] to create 16-bit int
  // Print Distance and axis pos
  Serial.print(posX);
  Serial.print(",");
  Serial.print(posY);
  Serial.print(",");
  Serial.println(distance);
}
