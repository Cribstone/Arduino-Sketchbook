/* ==============================
 * This code, which assumes you're using the official Arduino Ethernet shield,
 * enables you to control your Arduino outputs from a web form 
 * (see http://community.pachube.com/ for more info)
 * 
 * Tested with Arduino 14
 *
 * Pachube is www.pachube.com - connect, tag and share real time sensor data
 * code by usman (www.haque.co.uk), may 2009
 * copy, distribute, change, whatever, as you like.
 *
 * v1.1 - added User-Agent & fixed HTTP parser for new Pachube headers
 * and check millis() for when it wraps around
 *
 * =============================== */


#include <Ethernet.h>
#include <string.h>
#undef int() // needed by arduino 0011 to allow use of stdio
#include <stdio.h> // for function sprintf


#define SHARE_FEED_ID              1872      // the Pachube feed ID that shares your sensor data 
#define CONTROL_FEED_ID            1260      // feed ID of the Pachube controller feed 
#define UPDATE_INTERVAL            10000     // if the connection is good wait 10 seconds before updating again - should not be less than 5
#define RESET_INTERVAL             10000     // if connection fails/resets wait 10 seconds before trying again - should not be less than 5

#define PACHUBE_API_KEY            "ENTER_YOUR_API_KEY_HERE" // fill in your API key 



byte mac[] = { 
  0xCC, 0xAC, 0xBE, 0xEF, 0xFE, 0x91 }; // make sure this is unique on your network
byte ip[] = { 
  192, 168, 0, 143   };                  // no DHCP so we set our own IP address
byte remoteServer[] = { 
  209,40,205,190 };            // pachube.com

int digital_state[10];        // array to hold variables we grab from the controller feed
int analog_state[4];          // array to hold variables we grab from the controller feed
int analog_pins[] = { 
  3, 5, 6, 9}; // array to hold analog pin numbers



void setup()
{
  Serial.begin(57600); 
  setupEthernet(); 

  for (int i = 0; i < 10; i++){
    pinMode(i, OUTPUT);
  }
}

void loop()
{

  // call 'pachube_in_out' at the beginning of the loop, handles timing, requesting
  // and reading. use serial monitor to view debug messages

  pachube_in_out();

  // set all non-PWM digital pins. NOTE: pins 0 and 1 used by Serial - so remove all 
  // serial communication in this sketch if you need to use those pins

  for (int i = 0; i < 10; i++){
    if (!( (i == analog_pins[0]) || (i == analog_pins[1]) || (i == analog_pins[2]) || (i == analog_pins[3]) )) {
      digitalWrite( i, digital_state[i] );
    }
  }

  // check whether the analog state has been set greater than 0 and if so analogWrite, otherwise digitalWrite

  for (int i = 0; i < 4; i++){
    if (analog_state[i] > 0) {
      analogWrite(analog_pins[i], analog_state[i]);
    } 
    else {
      digitalWrite( analog_pins[i], digital_state[analog_pins[i]] );
    }
  }

}

