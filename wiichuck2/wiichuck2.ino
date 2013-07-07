#include "Wire.h"
//#include "WiiChuckClass.h" //most likely its WiiChuck.h for the rest of us.
#include WiiChuck.h

WiiChuck chuck = WiiChuck();

void setup() {
  //nunchuck_init();
  Serial.begin(115200);
  chuck.begin();
  chuck.update();
  //chuck.calibrateJoy();
}

void loop() {
  delay(20);
  chuck.update(); 

  Serial.print(chuck.readJoyX());
    Serial.print(", ");  
  Serial.print(chuck.readJoyY());
    Serial.print(", ");  

  if (chuck.buttonZ) {
     Serial.print("Z");
  } else  {
     Serial.print("-");
  }

    Serial.print(", ");  

//not a function//  if (chuck.buttonC()) {
  if (chuck.buttonC) {
     Serial.print("C");
  } else  {
     Serial.print("-");
  }

    Serial.println();

}
