#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <util.h>
#include <ctype.h>


#define CLIENTID "ArduinoSensor"
#define TOPICNAME "switch/signal"
#define POLLINTERVAL 120000

#define PORT 80
//Connect to MQTT server
byte server1 [] = {192, 168, 1, 4};
byte mac[] = {0x5e, 0xa4, 0x18, 0xf0, 0x8a, 0xf6};

IPAddress arduinoIP(192, 168, 1, 67);
IPAddress dnsIP(203, 145, 184, 32); 
IPAddress gatewayIP(192, 168, 1, 1);
IPAddress subnetIP(255, 255, 255, 0);
EthernetServer server(PORT);

EthernetClient ethClient;
PubSubClient arduinoClient(server1, 8081, callback, ethClient);
void callback(char* topic, byte* payload, unsigned int length) {
 // handle message arrived
}
int inPin1 = 2; 
int inPin2 = 0;// the number of the input pin
int outPin = 7; // the number of the output pi

int state = HIGH; // the current state of the output pin
int reading; // the current reading from the input pin
int previous = LOW; // the previous reading from the input pin

// the follow variables are long's because the time, measured in milliseconds
// will quickly become a bigger number than can be stored in an int.
long time = 0;                   // the last time the output pin was toggled
long debounce = 200;      // the debounce time, increase if the output flickers

int state2 = HIGH; // the current state of the output pin
int reading2; // the current reading from the input pin
int previous2 = LOW; // the previous reading from the input pin

// the follow variables are long's because the time, measured in milliseconds
// will quickly become a bigger number than can be stored in an int.
long time2 = 0;                   // the last time the output pin was toggled
long debounce2 = 200;      // the debounce time, increase if the output flickers
void setup()
{
 Serial.begin(9600);
 Ethernet.begin(mac, arduinoIP, dnsIP, gatewayIP, subnetIP);
   pinMode(inPin1, INPUT);
 pinMode(inPin2, INPUT);
 pinMode(outPin, OUTPUT);
 beginConnection() ;
}

void beginConnection() {
Serial.begin(9600);
//Ethernet.begin(mac) ;
int connRC = arduinoClient.connect(CLIENTID) ;
if (!connRC) {
Serial.println(connRC) ;
Serial.println("Could not connect to MQTT Server");
Serial.println("Please reset the arduino to try again");
delay(100);
exit(-1);
}
else {
Serial.println("Connected to MQTT Server...");


}}

void loop()
{

   reading = digitalRead(inPin1);

 // if the input just went from LOW and HIGH and we've waited long enough
 // to ignore any noise on the circuit, toggle the output pin and remember
 // the time
 if (reading == HIGH && previous == LOW && millis() - time > debounce) {
   if (state == HIGH){
     state = LOW;
       arduinoClient.publish(TOPICNAME, "Button A is Pressed") ;
     Serial.println("Button A is Pressed and message published");}
   else
   {
     state = HIGH;
        arduinoClient.publish(TOPICNAME, "Button A is Pressed") ;
     Serial.println("Button A is Pressed and message published");
   }
   time = millis(); 
 }
 digitalWrite(outPin, state);
 
 previous = reading;
 
  reading2 = digitalRead(inPin2);

 // if the input just went from LOW and HIGH and we've waited long enough
 // to ignore any noise on the circuit, toggle the output pin and remember
 // the time
 if (reading2 == HIGH && previous2 == LOW && millis() - time2 > debounce2) {
   if (state2 == HIGH){
     state2 = LOW;
       arduinoClient.publish(TOPICNAME, "Button B is Pressed") ;
     Serial.println("Button B is Pressed and message published");}
   else
   {
     state2 = HIGH;
     arduinoClient.publish(TOPICNAME, "Button B is Pressed") ;
     Serial.println("Button B is Pressed and message published");
   }
   time2 = millis(); 
 }
 digitalWrite(outPin, state2);
 
 previous2 = reading2;
   
}
