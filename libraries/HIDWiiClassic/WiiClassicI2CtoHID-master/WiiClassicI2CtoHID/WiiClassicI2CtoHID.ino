/*
   WiiClassic Test Code

   This code prints the current controller status to the serial port.
   Button pressed calls poll whether the button was pressed since the last update call.
   as a result, it will just briefly print the last button pressed once.

   Tim Hirzel May 2008

 */

#include "Wire.h"

#include "WiiClassic.h"
#include "HIDWiiClassic.h"

WiiClassic wiiClassy = WiiClassic();

void setup() {
    Wire.begin();
    Serial.begin(9600);
    wiiClassy.begin();
    wiiClassy.update();
    HIDWiiClassic.begin();
}

void loop() {
    delay(100); // 1ms is enough to not overload the wii Classic, 100ms seems to ease the serial terminal a little
    wiiClassy.update();
    HIDWiiClassicKeys keys;// = calloc(1, sizeof(HIDWiiClassicKeys));
    keys.buttons = 0;

    Serial.print("Buttons:");

    if (wiiClassy.leftShoulderPressed()) {
        Serial.print("LS.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_L);
    }
    if (wiiClassy.rightShoulderPressed()) {
        Serial.print("RS.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_R);
    }
    if (wiiClassy.lzPressed()) {
        Serial.print("lz.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_ZL);
    }
    if (wiiClassy.rzPressed()) {
        Serial.print("rz.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_ZL);
    }
    if (wiiClassy.leftDPressed()) {
        Serial.print("LD.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_LEFT);
    }

    if (wiiClassy.rightDPressed()) {
        Serial.print("RD.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_RIGHT);
    }

    if (wiiClassy.upDPressed()) {
        Serial.print("UD.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_UP);
    }
    if (wiiClassy.downDPressed()) {
        Serial.print("DD.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_DOWN);
    }

    if (wiiClassy.selectPressed()) {
        Serial.print("select.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_SELECT);
    }

    if (wiiClassy.homePressed()) {
        Serial.print("home.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_HOME);
    }
    if (wiiClassy.startPressed()) {
        Serial.print("start.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_START);
    }

    if (wiiClassy.xPressed()) {
        Serial.print("x.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_X);
    }

    if (wiiClassy.yPressed()) {
        Serial.print("y.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_Y);
    }

    if (wiiClassy.aPressed()) {
        Serial.print("a.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_A);
    }

    if (wiiClassy.bPressed()) {
        Serial.print("b.");
        SET_WII_CLASSIC_BUTTON(keys, WII_CLASSIC_B);
    }

    Serial.println();
    Serial.print("right shoulder: ");
    Serial.println(wiiClassy.rightShouldPressure());
    Serial.print(" left shoulder: ");

    Serial.println(wiiClassy.leftShouldPressure());
    Serial.print("  left stick x: ");
    keys.left_x = wiiClassy.leftStickX();
    Serial.println(wiiClassy.leftStickX());

    Serial.print("  left stick y: ");
    keys.left_y = wiiClassy.leftStickY();
    Serial.println(wiiClassy.leftStickY());

    Serial.print(" right stick x: ");
    keys.right_x = wiiClassy.rightStickX();
    Serial.println(wiiClassy.rightStickX());

    Serial.print(" right stick y: ");
    keys.right_y = wiiClassy.rightStickY();
    Serial.println(wiiClassy.rightStickY());

    Serial.println("---");

    HIDWiiClassic.write(keys);
    Serial.println("WRITTEN");
}

