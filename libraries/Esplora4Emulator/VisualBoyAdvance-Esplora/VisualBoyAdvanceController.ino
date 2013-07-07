/* 
   Created Dec 27, 2012
   by w9jds (Jeremy Shore)

   Code to use the Arduino Esplora as the controller
   for the VisualBoyAdvance Emulator. Now all you have
   to do is click down on the joystick to change the mapping
   on the buttons! Switches the top two buttons between the
   trigger and the start and select button.
*/
#include <Esplora.h>

boolean lastbtnStates[9];
boolean secButtons = false; // decides when to map the buttons differently

const byte buttons[] = {JOYSTICK_DOWN, JOYSTICK_UP, JOYSTICK_LEFT, JOYSTICK_RIGHT, SWITCH_DOWN, SWITCH_UP, SWITCH_LEFT, SWITCH_RIGHT, JOYSTICK_SWITCH};
const char corspKeys[] = {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 'x', 's', 'a', 'z'};
const char seccorspKeys[] = {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 'x', KEY_RETURN, KEY_BACKSPACE, 'z'};

void setup() {
  Keyboard.begin();
};

void loop() {  
  for (byte Button = 0; Button <= 8; Button++) {  
    if (Button == 8) { //if checking the joystick switch
      boolean newState = Esplora.readButton(buttons[Button]);
      if (lastbtnStates[Button] != newState) {
        if (newState == PRESSED) { //if pressed toggle the boolean deciding which button configs to use
          secButtons = !secButtons;
        }
      }
      lastbtnStates[Button] = newState;
    }
    else {
      if (secButtons == false){   
        boolean newState = Esplora.readButton(buttons[Button]);
        if (lastbtnStates[Button] != newState) {
          if (newState == PRESSED)
            Keyboard.press(corspKeys[Button]);
          else if (newState == RELEASED)
            Keyboard.release(corspKeys[Button]);
        }
        lastbtnStates[Button] = newState; //saves last state of the button
      }
      else if(secButtons == true) {
        boolean newState = Esplora.readButton(buttons[Button]);
        if (lastbtnStates[Button] != newState) {
          if (newState == PRESSED)
            Keyboard.press(seccorspKeys[Button]);
          else if (newState == RELEASED)
            Keyboard.release(seccorspKeys[Button]);
        }
        lastbtnStates[Button] = newState; //saves last state of the button
      }
    }
  }
  delay(50);
}

