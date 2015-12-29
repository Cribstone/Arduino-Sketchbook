#include <MySensor.h>
#include <SPI.h>

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

#define NODE_ID 2
#define CHILD_ID 0
#define SKETCH_NAME "RGB_STRIP"
#define SKETCH_VERSION "1.0.0"
#define NODE_REPEAT false

MySensor gw;

long RGB_values[3] = {0, 0, 0};
float dimmer;

void setup() {


  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  gw.begin(incomingMessage, NODE_ID, NODE_REPEAT);
  gw.sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);
  gw.present(CHILD_ID, S_RGB_LIGHT, "RGB Strip", false);
  gw.request(CHILD_ID, V_RGB);
}

void loop() {
  gw.process();
}

void incomingMessage(const MyMessage &message) {

  if (message.type == V_RGB) {

    String hexstring = message.getString();
    long number = (long) strtol( &hexstring[0], NULL, 16);
    RGB_values[0] = number >> 16;
    RGB_values[1] = number >> 8 & 0xFF;
    RGB_values[2] = number & 0xFF;
  }
  if (message.type == V_DIMMER) {
    dimmer = message.getInt();
    analogWrite(RED_PIN, int(RGB_values[0] * (dimmer / 100)));
    analogWrite(GREEN_PIN, int(RGB_values[1] * (dimmer / 100)));
    analogWrite(BLUE_PIN, int(RGB_values[2] * (dimmer / 100)));
  }

  if (message.type == V_LIGHT) {
    if (message.getInt() == 0) {
      digitalWrite(RED_PIN, 0);
      digitalWrite(GREEN_PIN, 0);
      digitalWrite(BLUE_PIN, 0);

    }
    if (message.getInt() == 1) {
      analogWrite(RED_PIN, int(RGB_values[0] * (dimmer / 100)));
      analogWrite(GREEN_PIN, int(RGB_values[1] * (dimmer / 100)));
      analogWrite(BLUE_PIN, int(RGB_values[2] * (dimmer / 100)));
    }
  }
}

