/* Nanode + DS1820 digital thermometer --> Cosm.

Copyright 2012 Andrew Back, and Gregor van Egdom.

  - http://carrierdetect.com

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Based heavily on a sketch by gregor@krekr.nl, which was itself based on:
        
- PacubeV3_WithoutNode by WickedDevice: http://blog.wickeddevice.com/?p=260
- Dallas Temperature examples: http://milesburton.com/Dallas_Temperature_Control_Library
        
Requires Ethershield, OneWire and Dallas Temperature libraries.

Hardware setup:
        
- Nanode, or an Arduino with ENC ethernet shield
- DS1820 OneWire digital thermometer on pin 4 */
        
#include <EtherShield.h>
#include <OneWire.h>
#include <DallasTemperature.h>
        
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* Sketch configuration                                     *
* * * * * * * * * * * * * * * * * * * * * * * * * * ** * * */

#define ONE_WIRE_BUS 4                                              // Nanode/Arduino pin used for the 1-wire bus
#define SERIAL_BAUD_RATE 9600                                       // Serial port speed
#define MY_MAC_ADDRESS {0x54,0x55,0x58,0x10,0x00,0x25}              // Must be unique
#define USE_DHCP                                                    // Comment out to use static network parameters

#define COSM_API_KEY "your-secret-cosm-API-key" 		    // change this to your API key
#define HTTPFEEDPATH "/v2/feeds/50266"                              // Change this to the relative URL of your feed
#define DELAY_BETWEEN_COSM_POSTS_MS 2000L                        // Post frequency in milliseconds
        
#ifndef USE_DHCP                                                    // If not using DHCP you need to set the network parameters
  #define MY_IP_ADDRESS {192,168,  0, 25}
  #define MY_NET_MASK   {255,255,255,  0}
  #define MY_GATEWAY    {192,168,  0,  1}
  #define MY_DNS_SERVER {192,168,  0,  1}
#endif
        
// Cosm feed template. See: http://api.cosm.com/v2/

#define FEED_POST_MAX_LENGTH 256
static char feedTemplate[] = "{\"version\":\"1.0.0\",\"datastreams\":[{\"id\":\"ActualTemp\", \"current_value\":\"%d\"}]}";
static char feedPost[FEED_POST_MAX_LENGTH] = {0}; // This will hold the populated template
        
uint8_t fillOutTemplateWithSensorValues(uint16_t sensorValue1){
  snprintf(feedPost, FEED_POST_MAX_LENGTH, feedTemplate, sensorValue1);
  return (1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* End sketch configuration                                .*
* * * * * * * * * * * * * * * * * * * * * * * * * * ** * * */

#define FEEDHOSTNAME "api.cosm.com\r\nX-ApiKey: " COSM_API_KEY
#define FEEDWEBSERVER_VHOST "api.cosm.com"
#define BUFFER_SIZE 550

static uint8_t mymac[6] = MY_MAC_ADDRESS;
static uint8_t websrvip[4] = { 0, 0, 0, 0 };                        // Cosm server IP (resolved by DNS)
static char hoststr[150] = FEEDWEBSERVER_VHOST;
static uint8_t buf[BUFFER_SIZE+1];
  
#ifndef USE_DHCP
  // Use the provided static parameters
  static uint8_t myip[4]      = MY_IP_ADDRESS;
  static uint8_t mynetmask[4] = MY_NET_MASK;
  static uint8_t gwip[4]      = MY_GATEWAY;
  static uint8_t dnsip[4]     = MY_DNS_SERVER;
#else
  // Get IP parameters via DHCP
  static uint8_t dhcpsvrip[4] = { 0,0,0,0 };    
  static uint8_t myip[4]      = { 0,0,0,0 };
  static uint8_t mynetmask[4] = { 0,0,0,0 };
  static uint8_t gwip[4]      = { 0,0,0,0 };
  static uint8_t dnsip[4]     = { 0,0,0,0 };
#endif

long lastPostTimestamp;                                             // Variable holding timestamp of last post
boolean firstTimeFlag = true;
float DS1820Temp;                                                   // Variable to hold temperature reading

OneWire oneWire(ONE_WIRE_BUS);                                      // Set up a oneWire instance to communicate with 1-wire devices
DallasTemperature sensors(&oneWire);                                // Pass oneWire reference to Dallas Temperature
DeviceAddress DS1820Thermometer;                                    // Arrays to hold device address

EtherShield es=EtherShield();

void setup(){
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("Nanode + DS1820 digital thermometer, posting to Cosm");      
  sensors.begin();
  if (!sensors.getAddress(DS1820Thermometer, 0))
  Serial.println("Unable to find address for Device 0");
  sensors.setResolution(DS1820Thermometer, 12);                     // Set DS1820 thermometer to 12-bit resolution
  es.ES_enc28j60SpiInit();                                          // Initialise SPI interface (for Ethernet chip)
  es.ES_enc28j60Init(mymac, 8);                                     // initialize ENC28J60 (Ethernet)   

  #ifdef USE_DHCP
    acquireIPAddress();
  #endif
  printNetworkParameters();
        
  es.ES_init_ip_arp_udp_tcp(mymac,myip, 80);                        // Initialise the IP stack
  es.ES_client_set_gwip(gwip);                                      // Get gateway IP 
  es.ES_dnslkup_set_dnsip(dnsip);                                   // Get the DNS resolver IP
  Serial.println("Awaiting Client Gateway");
  while(es.ES_client_waiting_gw()){
    int plen = es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf);
    es.ES_packetloop_icmp_tcp(buf,plen);    
  }
  Serial.println("Client Gateway Complete, Resolving Host");
  resolveHost(hoststr, websrvip);
  Serial.print("Resolved host: ");
  Serial.print(hoststr);
  Serial.print(" to IP: ");
  printIP(websrvip);
  Serial.println();
  es.ES_client_set_wwwip(websrvip);       
  lastPostTimestamp = millis();
}
        
void loop(){
  long currentTime = millis();                                      // Timer used to trigger action
  int plen = es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf);         // Start receiving packets and buffer
  es.ES_packetloop_icmp_tcp(buf,plen);
  // Now post to Cost
  if(currentTime - lastPostTimestamp > DELAY_BETWEEN_COSM_POSTS_MS || firstTimeFlag){   
    firstTimeFlag = false;
    sensors.requestTemperatures();                                  // Send the command to read temperatures
    DS1820Temp = sensors.getTempC(DS1820Thermometer);               // Store temperature from the DS1820
    if(fillOutTemplateWithSensorValues(DS1820Temp)){                // If filling out the Cosm template is succesful, post.
      Serial.print("Posting sensor values to Cosm: ");
      Serial.print(DS1820Temp, DEC);
      Serial.println("");
      es.ES_client_http_post(PSTR(HTTPFEEDPATH),PSTR(FEEDWEBSERVER_VHOST),PSTR(FEEDHOSTNAME), PSTR("PUT "), feedPost, &sensor_feed_post_callback);  
    }
    lastPostTimestamp = currentTime;
  }
}
