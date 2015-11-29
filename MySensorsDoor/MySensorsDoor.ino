/ MySensorsDoor/Window sensor
// Based on Attiny85
// Revision 9/28/15 by BW
// Author: Wiebe Nieuwenhuis
// these are suggested wire colors but you can use any color you want,  just be consistent on all sensors
//                    +-\/-+
//  ORANGE   CE      1|o   |8  VCC   RED
//  YELLOW   CSN     2|    |7  SCK   GREEN
//  -        SENSOR  3|    |6  MOSI  BLUE
//  BLACK    GND     4|    |5  MISO  VIOLET
//                    +----+

#include <MySensor.h>
#include <Bounce2.h>

#define SENSOR_INFO "Door sensor"
#define NODE_ID 1 // Start numbering by 1 adding 1 each time you upload to new attiny85
#define CHILD_ID 1 // this number must match above number
#define INPUT_PIN 4 // This is ATiny input number pin # 3 is the physical pin

MySensor gw;
Bounce debouncer = Bounce(); 
int oldValue = -1;

MyMessage msg(NODE_ID, V_TRIPPED);

void setup()
{
  pinMode(INPUT_PIN, INPUT);
  digitalWrite(INPUT_PIN, HIGH);
  
  debouncer.attach(INPUT_PIN);
  debouncer.interval(5);
  
  gw.begin(NULL, NODE_ID, false, 0);
  gw.sendSketchInfo(SENSOR_INFO, "1.15");
  gw.present(CHILD_ID, S_DOOR);
}


void loop()
{
  debouncer.update();
  int value = debouncer.read();
  if (value != oldValue) {
     gw.send(msg.set(value==HIGH ? 1 : 0));
     oldValue = value;
  }
}

