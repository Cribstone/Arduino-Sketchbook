#include <SensuinoEth.h>
#include <SensuinoSerial.h>
#include <Ethernet.h>
#include <SPI.h>

SensuinoEth pf;

const long int device_id = 7420; // This is your device's ID


const long int feed_19867 = 19867; // This is the feed "iot-printer"


unsigned long timeRef;

int val = 0;
char buffer[30];

void setup() {
  Serial.begin(9600);  
  byte err;
  // Use the following if you don't want to use DHCP
  // pf.init(&err, 0, "aa:bb:cc:dd:ee:ff", "192.168.1.150", "192.168.1.1");// First IP is for the Arduino, second IP is for the gateway
  pf.init(&err, 1, "aa:bb:cc:dd:ee:ff");
  delay(1000);
  
  pf.setSenseKey("tstnnk_ZeI5X4wjMp5fjPg");// This is your sense key

  /* We set the PINs */
  
  
  pinMode(0, OUTPUT);
  

  Serial.println("Init done");

  // For each output feed, get the last value and trigger the right PIN on the arduino
  
  
  pf.getSense(feed_19867);
  Serial.println("Value for iot-printer");
  Serial.println(pf.getLastValue());
  // We convert the value received in integer
  val = atoi(pf.getLastValue());
  // We publish the result on PIN0
  
  
  if (val == 0) {
      digitalWrite(0, LOW);
  } else {
      digitalWrite(0, HIGH);
  }
  
  
  
  
  //We listen for incoming events on the device
  pf.longPollSense(device_id);
  
  // Init timeRef
  timeRef = millis();
}

void loop() {
  byte _check = pf.longPollCheck();
    if(_check == 0) {
      pf.longPollSense(device_id);
      
      int feed_id = pf.getLastFeedId();
      char *str = pf.getLastValue();
      
      
      
      if (feed_id == feed_19867) {
        Serial.println("Got a value for iot-printer");
        Serial.println(str);
        // We convert the value received in integer
        val = atoi(str);
        // We publish the result on PIN0
        
        
        if (val == 0) {
            digitalWrite(0, LOW);
        } else {
            digitalWrite(0, HIGH);
        }
        
      }
      
      
      
    } else if(_check == 2) {
      pf.longPollSense(device_id);
    }
    
    // Send the values from input feeds every minute or so
    if (millis() - timeRef > 60000) {
      
      
      
      timeRef = millis();
    }
    
    delay(200);
}
