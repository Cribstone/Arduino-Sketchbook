#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

struct RGB {
  byte r;
  byte g;
  byte b;
};

//*************Editable Options ******************************
//The colour of the "12" to give visual reference to the top
RGB Twelve = { 140, 0, 140 }; //purple
//All the other pixels with no information
RGB Background = { 51, 153, 255 };//blue
//The Second hand
RGB Second = { 255, 128, 0 };//orange
//The Minute hand
RGB Minute = { 255, 51, 51 };//red
//The Hour hand
RGB Hour = { 76, 153, 0 };//green
//Since the minute hand only does increments of 5, this is the number of minutes MORE to add.
//So you can tell its 10:57 if the hour hand is on 10, the minute hand is on 55 and there are 2 "Minutes" added
RGB Minutes = { 255, 255, 102 };//yellow

int hours_Offset_From_GMT = -7;
int brightness = 35;
char ssid[] = "Your SSID";  //  your network SSID (name)
char pass[] = "Your Password";// your network password

//************************************************************

// Which pin on the ESP8266 is connected to the NeoPixels?
#define PIN            5

unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

//Unix time in seconds
unsigned long epoch = 0;
unsigned long LastNTP = 0;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

int hour;
int minute;
int second;

int HourLight;
int MinuteLight;
int MinutesLights;
int SecondLight;

int RequestedTime = 0;
int TimeCheckLoop = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pixels.setBrightness(brightness);
  Draw_Background();
  pixels.show();

  ConnectToAP();

  //Get the time
  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  Request_Time();
  delay(2000);
  while (!Check_Time())
  {
    delay(2000);
    TimeCheckLoop++;
    if (TimeCheckLoop > 5)
    {
      Request_Time();
    }
  }
}

void ConnectToAP()
{
  Serial.println("Attempting to Connect");
  randomSeed(analogRead(6));
  RGB RandomLights {((byte)random(0, 256)), ((byte)random(0, 256)), ((byte)random(0, 256))};

  while (true)
  {
    delay(1000);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    RandomLights = {((byte)random(0, 256)), ((byte)random(0, 256)), ((byte)random(0, 256))};
    for (int i = 0; i < 12; i++) {
      pixels.setPixelColor(i, pixels.Color(RandomLights.r, RandomLights.g, RandomLights.b));
      pixels.show();
      delay(100);
    }
    WiFi.begin(ssid, pass);
    for (int x = 0; x < 5; x++)
    {
      RandomLights = {((byte)random(0, 256)), ((byte)random(0, 256)), ((byte)random(0, 256))};
      for (int i = 0; i < 12; i++) {
        pixels.setPixelColor(i, pixels.Color(RandomLights.r, RandomLights.g, RandomLights.b));
        pixels.show();
        delay(100);
      }
      delay(1000);
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.print("WiFi connected in ");
        Serial.print(x);
        Serial.println(" seconds");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        return;
      }
    }
  }
}

void loop() {


  //Serial.print("Epoch is: ");
  //Serial.println(epoch);
  int SecondsSinceLastNTP = (millis() - LastNTP) / 1000;
  Serial.print("Seconds since last NTP: ");
  Serial.println(SecondsSinceLastNTP);

  //Update NTP every 2 minutes
  if (SecondsSinceLastNTP > 120 and RequestedTime == 0) {
    Request_Time();
    RequestedTime = 1;
  }

  if (RequestedTime == 1)
  {
    Check_Time();
    TimeCheckLoop++;
    if (TimeCheckLoop > 5)
    {
      RequestedTime = 0;
    }
  }

  //Add seconds to Epoch using ESP8266 Millis as a reference
  //This way the unit keeps time between calls to NTP
  //Only Check_Time() should update epoch
  DecodeEpoch(epoch + SecondsSinceLastNTP);

  DrawTime();


  delay(500);
}

void DrawTime()
{
  MinutesLights = minute % 5;
  MinuteLight = minute / 5;
  SecondLight = second / 5;
  if (hour == 12)
  {
    hour = 0;
  }

  //Start with the background
  RGB digits[12] = {Twelve, Background, Background, Background, Background, Background, Background, Background, Background, Background, Background, Background};

  //Update for minutes
  for (int i = 1; i <= MinutesLights; i++) {
    digits[i] = Minutes;
  }
  digits[MinuteLight] = Minute;
  digits[SecondLight] = Second;
  digits[hour] = Hour;

  for (int i = 0; i <= 12; i++) {
    pixels.setPixelColor(i, pixels.Color(digits[i].r, digits[i].g, digits[i].b));
  }
  pixels.show();
}

void Draw_Background()
{
  for (int i = 0; i < 12; i++) {
    pixels.setPixelColor(i, pixels.Color(Background.r, Background.g, Background.b));
  }
  //Twelve o'clock pixel
  pixels.setPixelColor(0, pixels.Color(Twelve.r, Twelve.g, Twelve.b));
  pixels.show();
}


//Get Time

void DecodeEpoch(unsigned long currentTime)
{
  // print the hour, minute and second:
  Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
  Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
  Serial.print(':');
  if ( ((epoch % 3600) / 60) < 10 ) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
  Serial.print(':');
  if ( (epoch % 60) < 10 ) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.println(epoch % 60); // print the second

  //Update for local zone
  currentTime = currentTime + (hours_Offset_From_GMT * 60 * 60);
  Serial.print("The current local time is ");
  Serial.print((currentTime  % 86400L) / 3600); // print the hour (86400 equals secs per day)
  Serial.print(':');
  if ( ((currentTime % 3600) / 60) < 10 ) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.print((currentTime  % 3600) / 60); // print the minute (3600 equals secs per minute)
  Serial.print(':');
  if ( (currentTime % 60) < 10 ) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    Serial.print('0');
  }
  Serial.println(currentTime % 60); // print the second




  hour = (currentTime  % 86400L) / 3600;
  if (hour > 12)
  {
    hour -= 12;
  }
  minute = (currentTime % 3600) / 60;
  second = currentTime % 60;
  MinutesLights = minute % 5;
}

void Request_Time()
{
  Serial.println("Getting Time");
  sendNTPpacket(timeServer); // send an NTP packet to a time server
}

bool Check_Time()
{
  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
    return false;
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = " );
    // Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    //Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    epoch = secsSince1900 - seventyYears;
    LastNTP = millis();
    RequestedTime = 0;
    TimeCheckLoop = 0;
    return true;
  }
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress & address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
