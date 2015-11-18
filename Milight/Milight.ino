#include <SPI.h>        
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <EtherShield.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(255, 255, 255, 255);
unsigned int localPort = 8899;
EthernetUDP Udp;     

#define pir A0

byte lightson[] = {0x22,0x00,0x55};
byte lightsoff[] = {0x21,0x00,0x55};
byte dim[] = {0x24,0x00,0x55};
byte bright[]={0x23,0x00,0x55};
byte prev[]={0x28,0x00,0x55};


IPAddress remoteIp1(192.168.1.24);        
unsigned int remotePort1 = 8899; 
boolean lightstatus=true;
boolean startup=true;

void setup() {
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.begin(9600);
  pinMode(pir, INPUT);
}

void loop() {
  if (startup==true)
  {
    delay (100);
     Udp.beginPacket(remoteIp1, remotePort1);
    Udp.write(lightson,3);
    Udp.endPacket();
     for (int i = 1; i < 20; i++)
            {
               
               Udp.beginPacket(remoteIp1, remotePort1);
               Udp.write(prev,3);
               Udp.endPacket();
               delay(50);
            }   

     Udp.beginPacket(remoteIp1, remotePort1);
    Udp.write(lightsoff,3);
    Udp.endPacket();  
    startup=false;
  }

if (lightstatus==true)  
  {
    if (digitalRead(pir) == 0)
    {
      Serial.println("Room Empty");
      lightoff();
    }

  }
  if (digitalRead(pir) == 1)
  {
    Serial.println("Motion Sensed");
    lighton();  
  }

  delay(100);
}
void lightoff(){
   Serial.println("This is dimming level " );
  for (int i = 1; i < 8; i++)
            {
               
               Udp.beginPacket(remoteIp1, remotePort1);
               Udp.write(dim,3);
               Udp.endPacket();
               delay(50);
            }   
            
    Udp.beginPacket(remoteIp1, remotePort1);
    Udp.write(lightsoff,3);
    Udp.endPacket();
    lightstatus=false;
    Serial.println("Light should be OFF" );
}

void lighton(){
   Serial.println("This is brightening level " );
    Udp.beginPacket(remoteIp1, remotePort1);
    Udp.write(lightson,3);
    Udp.endPacket();
    Serial.println("Light should be ON" );
    delay(50);
    for (int i = 1; i < 15; i++)
            {
               Udp.beginPacket(remoteIp1, remotePort1);
               Udp.write(bright,3);
               Udp.endPacket();
               delay(50);
            }   
   
    delay(10000);
    for (int i = 1; i < 10000; i++)
    {
      if (digitalRead(pir) == 1)
        {
          i==0;
        }
          delay(10);
    }   

      lightstatus=true;
}
