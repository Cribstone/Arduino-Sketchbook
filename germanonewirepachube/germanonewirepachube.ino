#include <SPI.h>
#include <Ethernet.h>
#include <Cosm.h>
#include <OneWire.h>

OneWire  ds(8);  // on pin 8

byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0xCF, 0x4F};
char apiKey[] = "wPVeY8Paj3x4cEH7uzJlTaoHrd6SAKxDVmtBaUdOMk5Iaz0g";
long feedId =  87577;
char datastreamId[] = "Upstairs";
long prevMillis = 0;
float Fahrenheit;
CosmClient client = CosmClient(apiKey);

void setup()
{
  Serial.begin(9600);
  client.connectWithMac(mac);
}

void loop()
{
 
 unsigned long curMillis = millis();
 if(curMillis - prevMillis > 10000)   
  {
   
     byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  
  
  if ( !ds.search(addr)) {
    
    ds.reset_search();
    delay(250);
    return;
  }
  
  

 
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

 
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    
  }
  

  // convert the data to actual temperature

  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  Fahrenheit = (float)raw / 16.0;
  
  Serial.print("Temperatur = ");
  Serial.print(Fahrenheit);
  Serial.print(" Fahrenheit, ");
  //ende tempmessung  
  prevMillis = curMillis;
  }
    else{
    // send to the Pachube client
    client.updateFeed(feedId, datastreamId, Fahrenheit);
    }
}

