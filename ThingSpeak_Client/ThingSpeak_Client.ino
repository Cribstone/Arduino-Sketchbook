/*

 ThingSpeak Client to Update Channel Feeds
 
 The ThingSpeak Client sketch is designed for the Arduino + Ethernet Shield.
 This sketch updates a channel feed with an analog input reading via the
 ThingSpeak API (http://community.thingspeak.com/documentation/)
 using HTTP POST.
 
 Getting Started with ThingSpeak:
 
    * Sign Up for New User Account - https://www.thingspeak.com/users/new
    * Create a New Channel by selecting Channels and then Create New Channel
    * Enter the Write API Key in this sketch under "ThingSpeak Settings"
 
 Created: January 25, 2011 by Hans Scharler (http://www.iamshadowlord.com)
 
 Additional Credits: Example sketches from Tom Igoe and David A. Mellis
 
*/

#include <SPI.h>
#include <Ethernet.h>

// Local Network Settings
byte mac[]     = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network
byte ip[]      = { 192, 168,   0,  249 };                // Must be unique on local network
byte gateway[] = { 192, 168,   0,   1 };
byte subnet[]  = { 255, 255, 255,   0 };

// ThingSpeak Settings
byte server[]  = { 184, 106, 153, 149 }; // IP Address for the ThingSpeak API
String writeAPIKey = "Write API Key";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 30000;        // Time interval in milliseconds to update ThingSpeak   
Client client(server, 80);

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int resetCounter = 0;

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  String analogPin0 = String(analogRead(A0), DEC);
  
  // Print Update Response to Serial Monitor
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
  
  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println();
    Serial.println("...disconnected.");
    Serial.println();
    
    client.stop();
  }
  
  // Update ThingSpeak
  if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("field1="+analogPin0);
  }
  
  lastConnected = client.connected();
}

void updateThingSpeak(String tsData)
{
  if (client.connect())
  { 
    Serial.println("Connected to ThingSpeak...");
    Serial.println();
        
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    
    resetCounter = 0;
    
  }
  else
  {
    Serial.println("Connection Failed.");   
    Serial.println();
    
    resetCounter++;
    
    if (resetCounter >=5 ) {resetEthernetShield();}

    lastConnectionTime = millis(); 
  }
}

void resetEthernetShield()
{
  Serial.println("Resetting Ethernet Shield.");   
  Serial.println();
  
  client.stop();
  delay(1000);
  
  Ethernet.begin(mac, ip, gateway, subnet);
  delay(1000);
}


