/*
Discover all DS18B20 devices on the OneWire bus and send their reading the Pachube
Use DHCP for configuration
Just change the API and FeedID and you are in business
2012, harizanov.com

*/
 
#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ERxPachube.h>
void PrintDataStream(const ERxPachube& pachube);

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xCF, 0x4F }; 
/* byte ip[] = { 192,168,0, 82 }; 
byte gateway[] = {192,168,0,1}; 
byte subnet[] = { 255, 255, 255, 0 };
*/

long LastFeedTime, loopTime, SensorsReadTime;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(4);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#define PACHUBE_API_KEY				"ZU9YElMQKZY6qj0sN_PC_8HuzYqSAKxndThjd3duN2RORT0g" // fill in your API key PACHUBE_API_KEY
#define PACHUBE_FEED_ID				87462 // fill in your feed id
int ledPin = 9;                 // LED connected to digital pin 9

static int numberOfDevices=0;

ERxPachubeDataOut dataout(PACHUBE_API_KEY, PACHUBE_FEED_ID);

void setup() {
  Serial.begin(9600);
  Serial.println("\n[DS18B20 sender to Pachube]");

  pinMode(ledPin, OUTPUT);      // sets the digital pin as output

  blink(8,50);  //Show we are alive
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      blink(127,30);
  }
  
   delay(1000);
   
   // Start up the library
   sensors.begin();
   numberOfDevices = sensors.getDeviceCount();

   blink(numberOfDevices,200);
      
   Serial.print("Found ");
   Serial.print(numberOfDevices);
   Serial.println(" devices");
   
   DeviceAddress tmp_address;
   
   for(int i=0;i<numberOfDevices; i++){
    sensors.getAddress(tmp_address, i);
    printAddress(tmp_address);
    Serial.println();
    dataout.addData(i);
   }  
  }

void loop()
{
  loopTime=millis();
  if (loopTime-LastFeedTime>59998) // push to pachube every minute
  {
    LastFeedTime=loopTime;   
    blink(3,150);
    feedPachube(); 
  }

  if (loopTime-SensorsReadTime>9999) // read sensors every 10 seconds
  {
    SensorsReadTime=loopTime;
    blink(2,150);
    sensorsRead();   
  }
}

void feedPachube(){

  sensors.requestTemperatures(); // Send the command to get temperatures
  delay(1000);
  
  for(int i=0;i<numberOfDevices; i++)
  {
    dataout.updateData(i,sensors.getTempCByIndex(i));
  }
  int status = dataout.updatePachube();

  Serial.print("sync status code <OK == 200> => ");
  Serial.println(status);
  PrintDataStream(dataout);
}



void sensorsRead(){

  sensors.requestTemperatures(); // Send the command to get temperatures
  delay(1000);
  
    for(int i=0;i<numberOfDevices; i++)
  {
    Serial.print("Sensor #"); Serial.print(i);  Serial.print(" ");
    Serial.print(sensors.getTempCByIndex(i));  Serial.println("C");
  }
}

  
void blink(int times, int howlong)
{
  for(int i=0;i<times;i++) {
  digitalWrite(ledPin, HIGH);   // sets the LED on
  delay(howlong);                  // waits for a second
  digitalWrite(ledPin, LOW);    // sets the LED off
  delay(howlong);                  // waits for a second
  }
}



void printAddress(DeviceAddress deviceAddress)
{
  Serial.print("{ ");
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    Serial.print("0x");
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    if (i<7) Serial.print(", ");
    
  }
  Serial.print(" }");
}

void PrintDataStream(const ERxPachube& pachube)
{
	unsigned int count = pachube.countDatastreams();
	Serial.print("data count=> ");
	Serial.println(count);

	Serial.println("<id>,<value>");
	for(unsigned int i = 0; i < count; i++)
	{
		Serial.print(pachube.getIdByIndex(i));
		Serial.print(",");
		Serial.print(pachube.getValueByIndex(i));
		Serial.println();
	}
}
