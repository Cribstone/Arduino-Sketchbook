

//Wiichuck Mouse
//Fork Robotics 2012
 
#include "Wire.h"
#include "nunchuck_funcs.h"
int loop_cnt=0;
 
// parameters for reading the joystick:
int range = 40;               // output range of X or Y movement
int threshold = range/10;      // resting threshold
int center = range/2;         // resting position value
 
boolean mouseIsActive = false;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state
 
void setup() {
  // initilization for the Wiichuck
  nunchuck_init();
  // take control of the mouse:
  Mouse.begin();
}
 
void loop() {
  if( loop_cnt > 10 ) { // every 10 msecs get new data
    loop_cnt = 0;
    nunchuck_get_data();
 
    //right and left click control
    int leftState = nunchuck_cbutton();
    if (leftState) Mouse.press(MOUSE_LEFT); else Mouse.release(MOUSE_LEFT);
    int rightState = nunchuck_zbutton();
    if (rightState) Mouse.press(MOUSE_RIGHT); else Mouse.release(MOUSE_RIGHT);
 
    // read the x axis
    int xReading = nunchuck_joyx();
    xReading = map(xReading, 38, 232, 0, range);
    int xDistance = xReading - center;
    if (abs(xDistance) < threshold) {
      xDistance = 0;
    }
 
    // read the y axis
    int yReading = nunchuck_joyy();
    yReading = map(yReading, 38, 232, 0, range);
    int yDistance = yReading - center;
    if (abs(yDistance) < threshold) {
      yDistance = 0;
    }
 
    // move the mouse
    if ((xDistance != 0) || (yDistance != 0)) {
      Mouse.move(xDistance, -yDistance, 0);
    }
  }
  loop_cnt++;
  delay(1);
}
