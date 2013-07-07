/* 
   Created Dec 27, 2012
   by w9jds (Jeremy Shore)

   Code to use the Arduino Esplora as the controller for
   the SNES9X Emulator. Now all you have to do is click down
   on the joystick to change the mapping on the buttons! Still
   set up as top = right trigger, left = left trigger, right = start,
   left = select and standard snes buttons. It is set up for the
   buttons that are automatically set by SNES9X. Also the LED lights
   up pink for the pink buttons and purple for the purple buttons.
*/

#include <Esplora.h>

boolean lastbtnStates[9];
boolean secButtons = false; // decides when to map the buttons differently

const byte buttons[] = {JOYSTICK_DOWN, JOYSTICK_UP, JOYSTICK_LEFT, JOYSTICK_RIGHT, SWITCH_DOWN, SWITCH_UP, SWITCH_LEFT, SWITCH_RIGHT, JOYSTICK_SWITCH};
const char corspKeys[] = {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 'c', 'd', 'x', 'v'};
const char seccorspKeys[] = {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_RETURN, 's', 'a', ' '};

void setup() {
  Keyboard.begin();
};

void loop() {
  //int slider = Esplora.readSlider(); //here if you want to change back to potentiometer
  //int SetKeyTypes = map(slider, 0, 1023, 0, 1); //here if you want to change back to potentiometer
  
  for (byte Button = 0; Button <= 8; Button++) {  
    if (Button == 8) { //if checking the joystick switch
      boolean newState = Esplora.readButton(buttons[Button]);
      if (lastbtnStates[Button] != newState) {
        if (newState == PRESSED)
          secButtons = !secButtons;
      }
      lastbtnStates[Button] = newState;
    }
    else {
      if (secButtons == false){   
        boolean newState = Esplora.readButton(buttons[Button]);
        if (lastbtnStates[Button] != newState) {
          if (newState == PRESSED) {
            Keyboard.press(corspKeys[Button]);
            if ((Button == 4) || (Button == 7))
              Esplora.writeRGB(148,0,211); // purple for bottom buttons }
            if ((Button == 5) || (Button == 6))
              Esplora.writeRGB(255,105,180); //pink for top buttons }
          }
          else if (newState == RELEASED) {
            Keyboard.release(corspKeys[Button]);
            Esplora.writeRGB(0,0,0); //turns off the leds
          }
        }
        lastbtnStates[Button] = newState; //saves last state of the button
      }
      else if(secButtons == true) {
        boolean newState = Esplora.readButton(buttons[Button]);
        if (lastbtnStates[Button] != newState) {
          if (newState == PRESSED) { Keyboard.press(seccorspKeys[Button]); }
          else if (newState == RELEASED) { Keyboard.release(seccorspKeys[Button]); }
        }
        lastbtnStates[Button] = newState; //saves last state of the button
      }
    }
  }
  delay(50);
}

