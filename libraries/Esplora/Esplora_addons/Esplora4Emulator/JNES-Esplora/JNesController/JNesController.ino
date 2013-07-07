/* 
   Created Dec 27, 2012
   by w9jds (Jeremy Shore)

   Standard settings for the controls. Bottom two switches are the a and b
   buttons and the top two are the start and select buttons. Joystick doesn't
   work as well as on the SNES emulator because doesn't accept diagonal movement
   on some games but it does well!
*/

#include <Esplora.h>

boolean lastbtnStates[8];

const byte buttons[] = {JOYSTICK_DOWN, JOYSTICK_UP, JOYSTICK_LEFT, JOYSTICK_RIGHT, SWITCH_DOWN, SWITCH_UP, SWITCH_LEFT, SWITCH_RIGHT};
const char corspKeys[] = {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 'd', 's', 'a', 'f'};

void setup() {
  Keyboard.begin();
};

void loop() {
  for (byte Button = 0; Button <= 7; Button++) {    
    boolean newState = Esplora.readButton(buttons[Button]);
    if (lastbtnStates[Button] != newState) {
      if (newState == PRESSED) {
        Keyboard.press(corspKeys[Button]);
        if ((Button == 4) || (Button == 7)){ Esplora.writeRGB(255,0,0); } // Red for bottom buttons }
      }
      else if (newState == RELEASED) {
        Keyboard.release(corspKeys[Button]);
        Esplora.writeRGB(0,0,0); //turns off the leds
      }
    }
    lastbtnStates[Button] = newState; //saves last state of the button
  }
  delay(50);
}
