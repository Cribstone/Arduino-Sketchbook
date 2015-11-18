#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Bridge.h>
#include <YunClient.h>
#include <Wire.h>
#include <Infineon.h>
#include <Console.h> 

long checkAlive = 0;

Adafruit_NeoPixel widget = Adafruit_NeoPixel(12, 7, NEO_GRB + NEO_KHZ800);
YunClient yunClient;
PubSubClient client("iot.eclipse.org", 1883, callback, yunClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // Check for messages on subscribed topics
  Console.print("Topic: ");
  Console.println(String(topic));

  if(String(topic) == "1cfee8dd0afc") {
    String value = String((char*)payload);

    if(value == "jancumps"){
      checkAlive = millis();

      Console.println("Widget heartbeat received");
    }
  }
}

void setup() {
  Bridge.begin();
  Console.begin();
  Console.println("Connected to console.");
  Wire.begin();

  widget.begin();
  widget.show();

  // connect to MQTT broker and subscribe to topic
  if (client.connect("arduinoYunClient")) {
    client.subscribe("1cfee8dd0afc");
    delay(50);
  }
}

void loop() {
  // check heartbeat
  receiveHeartbeat();
}

void receiveHeartbeat() {
  if(millis() - checkAlive >= 120000) {
    // turn widget off
    for(uint16_t i=0; i<widget.numPixels(); i++) {
      widget.setPixelColor(i, widget.Color(0, 0, 0));
    }
    widget.show();
  } else {
    //turn widget on
    for(uint16_t i=0; i<widget.numPixels(); i++) {
      if(i%3 == 0) {
        widget.setPixelColor(i, widget.Color(255, 0, 0));
      } else {
        widget.setPixelColor(i, widget.Color(0, 255, 0));
      }
    }
    widget.show();
  }
}
