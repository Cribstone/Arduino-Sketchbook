		
// Program: 28BYJ-48 stepper motor control - Angulo   
// Author: Arduino and Co.  
   
#include <CustomStepper.h>  
   
// Set the initial parameters stepper motor connection   
CustomStepper stepper ( 8 , 9 , 10 , 11 , (byte []) { 8 , B1000, B1100, B0100,
B0110, B0010, B0011, B0001, B1001}, 4075.7728395, 12, CW);  
   
void setup()  
{  
  // Set the engine speed   
  stepper.setRPM ( 12 );  
   // Set the number of steps per revolution   
  stepper.setSPR ( 4075.7728395 );  
}  
   
void loop()  
{  
 if (stepper.isDone())  
 {  
   // Interval drives   
   delay ( 2000 );  
    // Set the direction of rotation (CW = Hours)  
   stepper.setDirection(CW);  
   // Set the angle of rotation   
   stepper.rotateDegrees ( 90 );  
  }  

  stepper.run();  
}  
