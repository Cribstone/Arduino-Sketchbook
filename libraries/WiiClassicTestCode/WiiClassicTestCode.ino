/*
WiiClassic Test Code

This code prints the current controller status to the serial port.
Button pressed calls poll whether the button was pressed since the last update call.
as a result, it will just briefly print the last button pressed once.

Tim Hirzel May 2008

*/


#include "Wire.h"

#include "WiiClassic.h"

WiiClassic wiiClassy = WiiClassic();

void setup() {
  Wire.begin();
  Serial.begin(9600);
  wiiClassy.begin();
  wiiClassy.update();
}

void loop() {
  delay(100); // 1ms is enough to not overload the wii Classic, 100ms seems to ease the serial terminal a little
  wiiClassy.update();

  // 
  /* 
  // PRINT RAW BYTES  FOR DEBUG
   // STATUS IS FIRST FOUR BYTES
   for (int i = 0; i < 4; i ++) {
   for (int j = 7; j >= 0; j--) {
   if ((wiiClassy.getRawStatus()[i] & (1 << j)) == 0) {
   Serial.print("0");
   }
   else{
   Serial.print("1");
   }
   }
   Serial.println();
   }
   // BUTTONS IS NEXT TWO BYTES
   for (int i = 0; i < 2; i ++) {
   for (int j = 7; j >= 0; j--) {
   if ((wiiClassy.getRawButtons()[i] & (1 << j)) == 0) {
   Serial.print("0");
   }
   else{
   Serial.print("1");
   }
   }
   Serial.println();
   }
  Serial.println("---");
   */

  Serial.print("Buttons:");


  if (wiiClassy.leftShoulderPressed()) {
    Serial.print("LS.");
  }
  if (wiiClassy.rightShoulderPressed()) {
    Serial.print("RS.");
  }
  if (wiiClassy.lzPressed()) {
    Serial.print("lz.");
  }
  if (wiiClassy.rzPressed()) {
    Serial.print("rz.");
  }
  if (wiiClassy.leftDPressed()) {
    Serial.print("LD.");
  }

  if (wiiClassy.rightDPressed()) {
    Serial.print("RD.");
  }

  if (wiiClassy.upDPressed()) {
    Serial.print("UD.");
  }
  if (wiiClassy.downDPressed()) {
    Serial.print("DD.");
  }

  if (wiiClassy.selectPressed()) {
    Serial.print("select.");
  }

  if (wiiClassy.homePressed()) {
    Serial.print("home.");
  }
  if (wiiClassy.startPressed()) {
    Serial.print("start.");
  }

  if (wiiClassy.xPressed()) {
    Serial.print("x.");
  }

  if (wiiClassy.yPressed()) {
    Serial.print("y.");
  }

  if (wiiClassy.aPressed()) {
    Serial.print("a.");
  }

  if (wiiClassy.bPressed()) {
    Serial.print("b.");
  }

  Serial.println();
  Serial.print("right shoulder: ");
  Serial.println(wiiClassy.rightShouldPressure());
  Serial.print(" left shoulder: ");

  Serial.println(wiiClassy.leftShouldPressure());
  Serial.print("  left stick x: ");

  Serial.println(wiiClassy.leftStickX());
  Serial.print("  left stick y: ");

  Serial.println(wiiClassy.leftStickY());
  Serial.print(" right stick x: ");

  Serial.println(wiiClassy.rightStickX());
  Serial.print(" right stick y: ");

  Serial.println(wiiClassy.rightStickY());
  Serial.println("---");

  //pulse = (int)500+8*wiiClassy.leftStickX();
  //updateServo();
}
