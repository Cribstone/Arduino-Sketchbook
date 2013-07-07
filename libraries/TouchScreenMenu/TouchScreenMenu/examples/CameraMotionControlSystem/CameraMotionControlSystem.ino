/*
Camera Motion Control System For Timelapse Photography

Two BigEasy Driver boards connected in serial to an Arduino Mega 2560.
A SeeedStudio 2.8" TFT Touch Screen V1 (http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html?cPath=132_134) 
touch screen LCD connected to the Arduino Mega 2560.

Libraries:
TouchScreenMenu 1.2 - http://code.google.com/p/touch-screen-menu-library/

TFT and TouchScreen - http://www.seeedstudio.com/wiki/File:TFT_touch_1.0_Libraries.zip

This is included as an example of how to use the TouchScreenMenu library.  This is not complete, but shows
how to setup menus and other types of screens in a real world application.
*/

#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>
#include <EEPROM.h>

#define VERSION "1.0"
#define TITLE "Motion Control System"
#define DEGREES_PER_FULL_STEP 7.5 // degrees per full step of the motors
#define FULL_STEPS_PER_ROTATION (360/MOTOR_FULL_STEP) // number of full steps in 360 degrees
#define EXPOSURE_TIME_TYPE_SUBSECOND 0 // if the exposure type time is sub 1 second
#define EXPOSURE_TIME_TYPE_POSTSECOND 1 // if the exposure type time is 1 second or more
#define EXPOSURE_TIME_TYPE_BULB 2 // if the exposure time type is buld/user defined

//-------------------------------------------------------------------------------
// motor related
//-------------------------------------------------------------------------------
// motor pins connected to the BigEasy Driver boards, these are the pins used to control
// the motors via the BED boards.
#define MPIN_ENABLE    39 //This input is pulled down on the board with a 20K Ohm resistor. You can leave it disconnected, or you can drive it from your microcontroller. When low, the driver chip is enabled and the motor is energized. When high, the driver chip is still enabled, but all of the final motor drive circuits are disabled and so no current will flow to the motor.
#define MPIN_MS1       41 //These inputs are all tied high with 20K Ohm resistors. They set the microstep setting.
#define MPIN_MS2       43
#define MPIN_MS3       45
#define MPIN_RESET     47 //This input is tied high with a 20K Ohm resistor on the BED. If this signal is pulled low, the motor driver circuits are shut off, and the driver is reset. Only when this input is high will the driver chip enable the motor driver circuits and pay attention to the STEP input.
#define MPIN_SLEEP     49 //This input is tied high with a 20K Ohm resistor on the BED. To put the driver chip into a lower power mode (motor driver circuits shut off, charge pump shut down, etc.) pull this input low. When coming out of sleep, allow at least 1ms before sending STEP pulses.
#define MPIN_DIR1      51 //This input is tied high with a 20K Ohm resistor on the BED. When a STEP pulse is received, the driver chip looks at this input to determine which direction to take the step. When high, the motor will turn counterclockwise, and when low, the motor will turn clockwise.
#define MPIN_DIR2      37 //This input is tied high with a 20K Ohm resistor on the BED. When a STEP pulse is received, the driver chip looks at this input to determine which direction to take the step. When high, the motor will turn counterclockwise, and when low, the motor will turn clockwise.
#define MPIN_STEP      53 //This input is tied high with a 20K Ohm resistor on the BED. Each rising edge of this input will cause the stepper driver to advance one step in the direction specified by the DIR input. The STEP input must be high for at least 1us, and low for at least 1us.

#define DIR_FORWARD  LOW // motor "forward"
#define DIR_BACKWARD HIGH // motor "backward"

// These inputs are all tied high with 20K Ohm resistors. They set the
// microstep setting. Use the following table to know how to control 
// the microstep settings:
//
// MS1 MS2 MS3 Microstep Resolution Excitation Mode
// L   L   L   Full Step 2 Phase
// H   L   L   Half Step 1-2 Phase
// L   H   L   Quarter Step W1-2 Phase
// H   H   L   Either Step 2W1-2 Phase
// H   H   H   Sixteenth Step 4W1-2 Phase

// motor step mode pin settings according to the above table
int motorStepModeTable[][3] = {
  {LOW,  LOW,  LOW},
  {HIGH, LOW,  LOW},
  {LOW,  HIGH, LOW},
  {HIGH, HIGH, LOW},
  {HIGH, HIGH, HIGH}
};

// motor step mode divisors according to the above table
#define MOTOR_STEP_MODE_COUNT 5
int motorStepModes[MOTOR_STEP_MODE_COUNT] = {1, 2, 4, 8, 16};

//-------------------------------------------------------------------------------
// camera related
//-------------------------------------------------------------------------------
// values for the camera, in this case Nikon D300
#define SUBSECOND_EXPOSURE_TIME_COUNT 39
float subSecondExposureTimes[SUBSECOND_EXPOSURE_TIME_COUNT] = {1.3, 1.6, 2, 2.5, 3, 4, 5, 6, 8, 10, 13, 15, 20, 25, 30, 40, 50, 60, 80, 100, 125, 160, 200, 250, 320, 400, 500, 640, 800, 1000, 1250, 1600, 2000, 2500, 3200, 4000, 5000, 6400, 8000};
#define POSTSECOND_EXPOSURE_TIME_COUNT 15
float postSecondExposureTimes[POSTSECOND_EXPOSURE_TIME_COUNT] = {1, 1.3, 2, 2.5, 3, 4, 5, 6, 8, 10, 13, 15, 20, 25, 30};

//-------------------------------------------------------------------------------
// EEPROM stored values
//-------------------------------------------------------------------------------
// These are the sequence setup values stored in EEPROM
int motorStepModeIndex = 0; // 0=full step, 1=1/2, 2=1/4, 3=1/8, 4=1/16
int stepsPerFrame = 1; // number of steps of the motor (in the given step mode) taken before each frame
long pauseTimePostFrame = 3; // the time to wait, after taking a frame, before moving the motors again
int exposureTimeType = EXPOSURE_TIME_TYPE_SUBSECOND; 
int exposureTimeIndex = 20; // (index into the subSecondExposureTimes/postSecondExposureTimes arrays) 125th of a second
long bulbExposureTime = 125; // (number of milliseconds for a EXPOSURE_TIME_TYPE_BULB type exposure) 125th of a second

//-------------------------------------------------------------------------------
// Menus
//-------------------------------------------------------------------------------
// create the array of items for the main menu
TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("Setup Timing"),
  TouchScreenMenuItem("Run Sequence"),
  TouchScreenMenuItem("Settings"),
  TouchScreenMenuItem("About"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the setup timing menu
TouchScreenMenuItem setupTimingMenuItems[] = {
  TouchScreenMenuItem("Motor Step Mode"),
  TouchScreenMenuItem("Steps Per Frame"),
  TouchScreenMenuItem("Exposure Time"),
  TouchScreenMenuItem("Post Exposure Pause"),
  TouchScreenMenuItem("ENDOFMENU")
};

TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 10, 20, CENTERJ, TITLE);

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &mainMenu;

TouchScreenButton backFromCurrentSetup = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
//TouchScreenButton backFromAbout = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);

//-------------------------------------------------------------------------------
// Program logic
//-------------------------------------------------------------------------------

void setup(void) {
  Serial.begin(57600);
  
  // set the various pins as input or output
  setPinModes();
  
  // set up the TouchScreenMenu libary
  TSC.setBackColor(TSC.createColor(0, 0, 0)); // change the default background color
  TSC.init(); // make sure everything gets initialized
  
  // read in the stored settings from EEPROM
  readSettings();
  
  setMotorStepMode(motorStepModeIndex);
  setMotorDir(DIR_FORWARD);
  
  // display the main menu
  mainMenu.setTitleFontSize(1);
  mainMenu.setClearScreenOnDraw(true);
  curMenu->draw(); // put up the main menu
}

void loop(void) 
{
  // handle the current menu
  if(curMenu!=NULL)
  {
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
  }
  else
  {
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them was pressed
    checkButtons();
  }
}

//-------------------------------------------------------------------------------
// check various buttons and perform actions if any was pressed
//-------------------------------------------------------------------------------
void checkButtons()
{
  if(backFromCurrentSetup.process())
    curMenu = &mainMenu;
  /*else if(backFromAbout.process())
    curMenu = &mainMenu;*/
  
  if(curMenu != NULL)
    curMenu->draw();
}

//-------------------------------------------------------------------------------
// check to see if any menu item was pressed and do something
//-------------------------------------------------------------------------------
void checkMenuSelection(TouchScreenMenuItem *item) {
  boolean handled = false;
  if(item != NULL){
    // main menu items 
    if(curMenu == &mainMenu){
      if(!strcmp(item->getText(),"Settings"))
      {
        displayCurrentSetup();
        //TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"About"))
      {
        displayAbout();
        //TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
        curMenu->drawItem(item,false);
  }
}

//-------------------------------------------------------------------------------
// set up the pins modes
//-------------------------------------------------------------------------------
void setPinModes()
{
  pinMode(MPIN_MS1, OUTPUT);
  pinMode(MPIN_MS2, OUTPUT);
  pinMode(MPIN_MS3, OUTPUT);
  pinMode(MPIN_DIR1, OUTPUT);
  pinMode(MPIN_DIR2, OUTPUT);
  pinMode(MPIN_ENABLE, OUTPUT);
  pinMode(MPIN_RESET, OUTPUT);
  pinMode(MPIN_SLEEP, OUTPUT);
  pinMode(MPIN_STEP, OUTPUT);
}

//-------------------------------------------------------------------------------
// set the stepping mode for the motors
//-------------------------------------------------------------------------------
void setMotorStepMode(int motorStepModeIndex)
{
  digitalWrite(MPIN_MS1,motorStepModeTable[motorStepModeIndex][0]);
  digitalWrite(MPIN_MS2,motorStepModeTable[motorStepModeIndex][1]);
  digitalWrite(MPIN_MS3,motorStepModeTable[motorStepModeIndex][2]);
}

//-------------------------------------------------------------------------------
// set the motor directions
//-------------------------------------------------------------------------------
void setMotorDir(int motorDirection)
{
  digitalWrite(MPIN_DIR1,motorDirection); // motor 1 runs in the opposite direction of motor 2
  digitalWrite(MPIN_DIR2,(motorDirection==DIR_FORWARD?DIR_BACKWARD:DIR_FORWARD)); // motor 2 runs in the opposite direction of motor 1
}

//-------------------------------------------------------------------------------
// display the about screen
//-------------------------------------------------------------------------------
void displayAbout()
{
  TouchScreenForm form = TouchScreenForm("About", 2);
  form.setClearScreenOnDraw(true);
  
  char versionStr[100];
  strcpy(versionStr,"Version: ");
  strcat(versionStr,VERSION);
  
  TouchScreenLabel labels[] = {
    TouchScreenLabel(TITLE, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 35, 1, 2, true),
    TouchScreenLabel("Written By: Scott Shaver", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 50, 1, 2, true),
    TouchScreenLabel(versionStr, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 65, 1, 2, true),
    TouchScreenLabel("ENDOFFORM")
  };
  
  TouchScreenButton buttons[] = {
    TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
    TouchScreenButton("ENDOFFORM")
  };

  form.setLabels(labels);
  form.setButtons(buttons);
  form.draw();

  while(1){ // stay on this screen until the back button is hit
    TouchScreenArea *item = form.process(true);
    if(item!=NULL){
      if(!strcmp(item->getText(),"<- Back")){
        return;
      }
    }
  }
}

//-------------------------------------------------------------------------------
// display the current settings
//-------------------------------------------------------------------------------
void displayCurrentSetup()
{
  TouchScreenForm form = TouchScreenForm("Settings", 2);
  form.setClearScreenOnDraw(true);
  
  char buffer[50];
  char buffer2[50];
  strcpy(buffer,"Motor Step Mode: 1/");
  strcat(buffer,itoa(motorStepModes[motorStepModeIndex],buffer2,10));
  TouchScreenLabel lbl1 = TouchScreenLabel(buffer, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 35, 1, 2, true);

  char buffer3[50];
  char buffer4[50];
  strcpy(buffer3,"Steps Per Frame: ");
  strcat(buffer3,itoa(stepsPerFrame,buffer4,10));
  TouchScreenLabel lbl2 = TouchScreenLabel(buffer3, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 50, 1, 2, true);

  char buffer5[50];
  char buffer6[50];
  TouchScreenLabel lbl3 = NULL;
  if(exposureTimeType == EXPOSURE_TIME_TYPE_SUBSECOND)
  {
    lbl3 = TouchScreenLabel("Sub-second Exposure Time -", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 65, 1, 2, true);
    dtostrf(subSecondExposureTimes[exposureTimeIndex], 5, 3, buffer6);
    sprintf(buffer5,"  Exposure Time: %s",buffer6);
  }
  else if(exposureTimeType == EXPOSURE_TIME_TYPE_POSTSECOND)
  {
    lbl3 = TouchScreenLabel("Post-second Exposure Time -", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 65, 1, 2, true);
    dtostrf(postSecondExposureTimes[exposureTimeIndex], 5, 3, buffer6);
    sprintf(buffer5,"  Exposure Time: %s",buffer6);
  }
  else if(exposureTimeType == EXPOSURE_TIME_TYPE_BULB) // bulb/user set time
  {
    lbl3 = TouchScreenLabel("Bulb Exposure Time -", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 65, 1, 2, true);
    strcpy(buffer5,"  Exposure Time: ");
    strcat(buffer5,itoa(bulbExposureTime,buffer6,10));
  }
  TouchScreenLabel lbl4 = TouchScreenLabel(buffer5, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 80, 1, 2, true);

  char buffer7[50];
  char buffer8[50];
  strcpy(buffer7,"Post Exposure Pause: ");
  strcat(buffer7,itoa(pauseTimePostFrame,buffer8,10));
  TouchScreenLabel lbl5 = TouchScreenLabel(buffer7, TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 5, 95, 1, 2, true);
  
  TouchScreenLabel labels[] = {
    lbl1,
    lbl2,
    lbl3,
    lbl4,
    lbl5,
    TouchScreenLabel("ENDOFFORM")
  };
  
  TouchScreenButton buttons[] = {
    TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(255, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
    TouchScreenButton("ENDOFFORM")
  };

  form.setLabels(labels);
  form.setButtons(buttons);
  form.draw();

  while(1){ // stay on this screen until the back button is hit
    TouchScreenArea *item = form.process(true);
    if(item!=NULL){
      if(!strcmp(item->getText(),"<- Back")){
        return;
      }
    }
  }
}

//-------------------------------------------------------------------------------
// read in the stored settings from the EEPROM
//-------------------------------------------------------------------------------
void readSettings()
{
  int address=0;
  Serial.println("Reading settings from EEPROM.");
  // read in the values from EEPROM
  // if not stored then set to defaults
  int value = EEPROM.read(address);
  if(value==255) // not stored
  {
    address+=2; // skip this memory
    motorStepModeIndex = 0;
  }
  else
    motorStepModeIndex = (EEPROM.read(address++)<<8) | EEPROM.read(address++);
  Serial.print("motorStepModeIndex=");
  Serial.println(motorStepModeIndex,DEC);
  setMotorStepMode(motorStepModeIndex);
  
  value = EEPROM.read(address);
  if(value==255) // not stored
  {
    address+=2; // skip this memory
    stepsPerFrame = 1;
  }
  else
    stepsPerFrame = (EEPROM.read(address++)<<8) | EEPROM.read(address++);
  Serial.print("stepsPerFrame=");
  Serial.println(stepsPerFrame,DEC);
    
  value = EEPROM.read(address);
  if(value==255) // not stored
  {
    address+=2; // skip this memory
    exposureTimeType = EXPOSURE_TIME_TYPE_SUBSECOND;
  }
  else
    exposureTimeType = (EEPROM.read(address++)<<8) | EEPROM.read(address++);
  Serial.print("exposureTimeType=");
  Serial.println(exposureTimeType,DEC);
    
  value = EEPROM.read(address);
  if(value==255) // not stored
  {
    address+=2; // skip this memory
    exposureTimeIndex = 0;
  }
  else
    exposureTimeIndex = (EEPROM.read(address++)<<8) | EEPROM.read(address++);
  Serial.print("exposureTimeIndex=");
  Serial.println(exposureTimeIndex,DEC);
    
  value = EEPROM.read(address);
  if(value==255) // not stored
  {
    address+=4; // skip this memory
    bulbExposureTime = 125;
  }
  else
    bulbExposureTime = (EEPROM.read(address++)<<24) | (EEPROM.read(address++)<<16) | (EEPROM.read(address++)<<8) | EEPROM.read(address++);
  Serial.print("bulbExposureTime=");
  Serial.println(bulbExposureTime,DEC);
  
  value = EEPROM.read(address);
  if(value==255) // not stored
  {
    address+=4; // skip this memory
    pauseTimePostFrame = 0;
  }
  else
    pauseTimePostFrame = (EEPROM.read(address++)<<24) | (EEPROM.read(address++)<<16) | (EEPROM.read(address++)<<8) | EEPROM.read(address++);
  Serial.print("pauseTimePostFrame=");
  Serial.println(pauseTimePostFrame,DEC);
}

//-------------------------------------------------------------------------------
// write settings to the EEPROM
//-------------------------------------------------------------------------------
void writeSettings()
{
  int address=0;
  Serial.println("Writting settings to EEPROM.");
  EEPROM.write(address++,(byte)(motorStepModeIndex>>8));
  EEPROM.write(address++,(byte)motorStepModeIndex);
  Serial.print("motorStepModeIndex=");
  Serial.println(motorStepModeIndex,DEC);
  
  EEPROM.write(address++,(byte)(stepsPerFrame>>8));
  EEPROM.write(address++,(byte)stepsPerFrame);
  Serial.print("stepsPerFrame=");
  Serial.println(stepsPerFrame,DEC);
    
  EEPROM.write(address++,(byte)(exposureTimeType>>8));
  EEPROM.write(address++,(byte)exposureTimeType);
  Serial.print("exposureTimeType=");
  Serial.println(exposureTimeType,DEC);
    
  EEPROM.write(address++,(byte)(exposureTimeIndex>>8));
  EEPROM.write(address++,(byte)exposureTimeIndex);
  Serial.print("exposureTimeIndex=");
  Serial.println(exposureTimeIndex,DEC);
    
  EEPROM.write(address++,(byte)(exposureTimeIndex>>24));
  EEPROM.write(address++,(byte)(exposureTimeIndex>>16));
  EEPROM.write(address++,(byte)(exposureTimeIndex>>8));
  EEPROM.write(address++,(byte)exposureTimeIndex);
  Serial.print("bulbExposureTime=");
  Serial.println(bulbExposureTime,DEC);
  
  EEPROM.write(address++,(byte)(pauseTimePostFrame>>24));
  EEPROM.write(address++,(byte)(pauseTimePostFrame>>16));
  EEPROM.write(address++,(byte)(pauseTimePostFrame>>8));
  EEPROM.write(address++,(byte)pauseTimePostFrame);
  Serial.print("pauseTimePostFrame=");
  Serial.println(pauseTimePostFrame,DEC);
}

