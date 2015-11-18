#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Bridge.h>
#include <YunClient.h>
#include <Wire.h>
#include <Infineon.h>
#include <Console.h> 

long isAlive = 0;

YunClient yunClient;
PubSubClient client("iot.eclipse.org", 1883, callback, yunClient);

void setup() {
  Bridge.begin();
  Console.begin();
  Console.println("Connected to console.");
  Wire.begin();

  // connect to MQTT broker and subscribe to topic
  if (client.connect("arduinoYunClient")) {
    client.publish("19e5983adc0f","fvan");
    isAlive = millis();
    delay(50);
  }
}

void loop() {
  // send heartbeat
  sendHeartbeat();
}

void sendHeartbeat() {
if(millis() - isAlive >= 30000) {
  client.publish("19e5983adc0f","fvan");
  isAlive = millis();
}
}
