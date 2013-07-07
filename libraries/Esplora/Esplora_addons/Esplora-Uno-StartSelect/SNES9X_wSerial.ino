/* 
   Created Dec 27, 2012
   by w9jds (Jeremy Shore)
*/

#include <Esplora.h>

boolean lastbtnStates[9];
boolean secButtons = false; // decides when to map the buttons differently

const byte buttons[] = {JOYSTICK_DOWN, JOYSTICK_UP, JOYSTICK_LEFT, JOYSTICK_RIGHT, SWITCH_DOWN, SWITCH_UP, SWITCH_LEFT, SWITCH_RIGHT, JOYSTICK_SWITCH};
const char corspKeys[] = {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 'c', 'd', 'x', 'v'};
const char seccorspKeys[] = {KEY_DOWN_ARROW, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, 'c', 's', 'a', 'v'};

void setup() {
  Serial1.begin(9400);
  Keyboard.begin();
};

void loop() {

  if (Serial1.available() > 0){ 
    String Input = Serial1.readStringUntil(';');
    if (Input == "Select"){
      Keyboard.press(KEY_RETURN);
      delay(20);
      Keyboard.release(KEY_RETURN);
    }
    if (Input == "Start"){
      Keyboard.press(' ');
      delay(20);
      Keyboard.release(' '); 
    } 
  }
 
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
              Esplora.writeRGB(148,0,211); // purple for bottom buttons
            if ((Button == 5) || (Button == 6))
              Esplora.writeRGB(255,105,180); //pink for top buttons
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

