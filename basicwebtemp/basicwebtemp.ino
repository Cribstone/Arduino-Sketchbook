/*
 Exosite Arduino Basic Temp Monitor 2 (updated to use Exosite library)
  
 This sketch shows an example of sending data from a connected
 sensor to Exosite. (http://exosite.com) Code was used from various
 public examples including the Arduino Ethernet examples and the OneWire
 Library examples found on the Arduino playground. 
 (OneWire Lib credits to Jim Studt, Tom Pollard, Robin James, and Paul Stoffregen)
  
 This code keeps track of how many milliseconds have passed
 and after the user defined REPORT_TIMEOUT (default 60 seconds)
 reports the temperature from a Dallas Semi DS18B20 1-wire temp sensor.
 The code sets up the Ethernet client connection and connects / disconnects 
 to the Exosite server when sending data.
  
 Assumptions:
 - Tested with Aruduino 1.0
 - Arduino included Ethernet Library
 - Arduino included SPI Library
 - Using Exosite library (2011-06-05) https://github.com/exosite-garage/arduino_exosite_library
 - Using OneWire Library Version 2.0 - http://www.arduino.cc/playground/Learning/OneWire
 - Using Dallas Temperature Control Library - http://download.milesburton.com/Arduino/MaximTemperature/DallasTemperature_372Beta.zip
 - User has an Exosite account and created a device (CIK needed / https://portals.exosite.com -> Add Device)
 - Uses Exosite's basic HTTP API, revision 1.0
  
 Hardware:
 - Arduino Duemilanove
 - Arduino Ethernet Shield
 - Dallas Semiconductor DS18B20 1-Wire Temp sensor used in parasite power mode (on data line, 4.7k pull-up)
 
 Version:
 1.0 - Novemeber 8, 2010 - by M. Aanenson
 2.0 - July 6, 2012 - by M. Aanenson
  
 */
  
  
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Exosite.h>
 
// User defined variables for Exosite reporting period and averaging samples
#define REPORT_TIMEOUT 120000 //milliseconds period for reporting to Exosite.com
#define SENSOR_READ_TIMEOUT 30000 //milliseconds period for reading sensors in loop
// Pin use
#define ONEWIRE 7 //pin to use for One Wire interface
 
// Set up which Arduino pin will be used for the 1-wire interface to the sensor
OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);
 
//global variables
byte macData[] = { "90-A2-DA-00-CF-4F"  }; // <-- Fill in your MAC here! (e.g. {0x90, 0xA2, 0xDA, 0x00, 0x22, 0x33}) 
String cikData = "24dfb175bbaa1099ce5dc4d708c8a4d576d56672";      // <-- Fill in your CIK here! (https://portals.exosite.com -> Add Device)
Exosite exosite(&Ethernet, macData, cikData);
 
 
//
// The 'setup()' function is the first function that runs on the Arduino.
// It runs completely and when complete jumps to 'loop()' 
//
void setup() {
  Serial.begin(9600);
 
  // Start up the OneWire Sensors library
  sensors.begin();
  delay(1000);
  Serial.println("Starting Exosite Temp Monitor");
  Serial.println();
   
  exosite.init();
  delay(2000); 
 
}
 
//
// The 'loop()' function is the 'main' function for Arduino 
// and is essentially a constant while loop. 
//
void loop() {
  static unsigned long sendPrevTime = 0;
  static unsigned long sensorPrevTime = 0; 
  static float tempF;
   
  Serial.print("."); // print to show running
 
 // Read sensor every defined timeout period
  if (millis() - sensorPrevTime > SENSOR_READ_TIMEOUT)
  {
    Serial.println();
    Serial.print("Requesting temperature...Temp: ");
    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempC = sensors.getTempCByIndex(0);
    tempF = DallasTemperature::toFahrenheit(tempC);
    Serial.print(tempF);
    Serial.println(" F ..........DONE");
     
    sensorPrevTime = millis();
  }
 
  // Send to Exosite every defined timeout period
  if (millis() - sendPrevTime > REPORT_TIMEOUT)
  {
    Serial.println(); //start fresh debug line
    Serial.println("Sending data to Exosite...");
     
    exosite.sendToCloud("temp",tempF);
 
    sendPrevTime = millis(); //reset report period timer
    Serial.println("done sending.");
  }
  delay(5000); //slow down loop
}
