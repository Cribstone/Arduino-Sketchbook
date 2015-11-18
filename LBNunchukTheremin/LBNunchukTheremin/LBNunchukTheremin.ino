/******************** (C) COPYRIGHT 2015 RadioShack ********************
 * File Name          : LBNunchukTheremin.ino
 * Author             : TRS
 * Version            : V1.0
 * Date               : 12/03/2015
 * Description        : littleBits theremin
 ********************************************************************************
 * THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, RS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *******************************************************************************/
#include <Wire.h>
#include <toneAC.h> //include toneAC library header file
#include "nunchuk.h" //library for nunchuk protocols, developed by Pragmatic Programmers

const unsigned int BAUD_RATE = 19200;

Nunchuk nunchuk;

void setup() {                

  pinMode( 9, OUTPUT); // sets output on littleBits Arduino Bit to Number bit
  pinMode( 1, OUTPUT ); // set pin 5 to OUTPUT mode
  pinMode( 5, OUTPUT ); // set pin 5 to OUTPUT mode


  Serial.begin(BAUD_RATE);
  nunchuk.initialize();
}

// ***********************************************************************************************************
// *
// *                            Main Loop 
// *
// *
// ***********************************************************************************************************
void loop() {

 
  if (nunchuk.update()) {  //Reads the sensor values from the nunchuk and sends to Serial Monitor. To view, select Tools > Serial Monitor menu.
    Serial.print(nunchuk.joystick_x()); 
    Serial.print(" ");
    Serial.print(nunchuk.joystick_y());
    Serial.print(" ");
    Serial.print(nunchuk.x_acceleration());
    Serial.print(" ");
    Serial.print(nunchuk.y_acceleration());
    Serial.print(" ");
    Serial.print(nunchuk.z_acceleration());
    Serial.print(" ");
    Serial.print(nunchuk.z_button());
    Serial.print(" ");
    Serial.println(nunchuk.c_button());


 if (nunchuk.c_button() == 1){                       //Higher tones when the C button is pressed.
    toneAC((nunchuk.x_acceleration() + (nunchuk.joystick_x() * 2)), (nunchuk.y_acceleration() / 70), 100, true); // Calculate tone by x acceleration and x joystick, tweak volume with y acceleration, sound for 100 milliseconds
  digitalWrite( 1, HIGH );                  // Turn on pin 5, optional LED to indicate pressed C button.
 }
else{
  digitalWrite( 1, LOW );             // Turn off pin 5.
} 
if (nunchuk.z_button() == 1){                     //Lower tones when the Z button is pressed.
    toneAC((nunchuk.x_acceleration() + (nunchuk.joystick_x() * 4)) / 2, (nunchuk.y_acceleration() / 70), 100, true); // Same as above, but divided tone by two for lower tones.
  digitalWrite( 5, HIGH );           // Turn on pin 5, optional LED to indicate pressed Z button.
 } 
else{
  digitalWrite( 5, LOW );            // Turn off pin 5.
} 
  }
}
