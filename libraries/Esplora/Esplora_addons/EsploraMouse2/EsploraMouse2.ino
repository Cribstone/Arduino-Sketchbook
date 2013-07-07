/* 
   Created Dec 31, 2012
   by w9jds (Jeremy Shore)

   This code sets up the Esplora to be used as your mouse
   it makes the potentiometer set how fast the mouse moves by 
   changing the threshold. It puts the left button for left click
   the bottom button for middle click and the right button for
   right click. The top button is to toggle if it is on or not.
   This one uses the accelerometer for movement instead of the 
   Joystick. 
*/
   
#include <Esplora.h>

boolean active = false;// whether or not to control the mouse
boolean prevStates[4];// previous switch state

const byte buttons[] = { SWITCH_UP, SWITCH_RIGHT, SWITCH_DOWN, SWITCH_LEFT };
const char mouseActions[] = { ' ', MOUSE_RIGHT, MOUSE_MIDDLE, MOUSE_LEFT };

void setup() {
  Mouse.begin();
}

void loop() {
  boolean newState = Esplora.readButton(buttons[0]);
  if (prevStates[0] != newState) {
    if (newState == PRESSED) {
      active = !active;
      if (active == true)
        Esplora.writeRGB(0,255,0);
      else
        Esplora.writeRGB(0,0,0);
    }
    prevStates[0] = newState;
  }

  // if the mouse control state is active, read the joystick and move the mouse:
  if (active == true) {
    int threshold = map(Esplora.readSlider(),0,1024,1000,20);
    int x = map(Esplora.readAccelerometer(X_AXIS), -1024, 1024, -threshold, threshold);
    int y = map(Esplora.readAccelerometer(Y_AXIS), -1024, 1024, -threshold, threshold);
    Mouse.move(-(x), (y), 0);
  }  

  for (byte z = 1; z <= 3; z++) {
    boolean newState = Esplora.readButton(buttons[z]);
    if (prevStates[z] != newState) {
      if (newState == PRESSED)
        Mouse.press(mouseActions[z]);
      else if (newState == RELEASED)
        Mouse.release(mouseActions[z]);
    }
    prevStates[z] = newState;
  }
  delay(50);
}
