#include <AndroidAccessory.h>

AndroidAccessory accessory("Circuits@Home, ltd.", "USB Host Shield");

void setup()
{
	Serial.begin(115200);
	Serial.println("\r\nADK demo start");
        
        if (!accessory.begin()) {
          Serial.println("OSCOKIRQ failed to assert");
          while(1); //halt
        }
}

void loop()
{
  uint8_t buf[ 12 ] = { 0 }; //buffer to convert unsigned long to ASCII
  const char* sec_ela = " seconds elapsed\r";
  uint8_t rcode;
  
   accessory.refresh();
   if(!accessory.isConnected()) {
     return;
   }
   
    ultoa( millis()/1000, (char *)buf, 10 );
     
    rcode = accessory.write(buf, strlen((char *)buf));
    rcode = accessory.write((uint8_t *)sec_ela, strlen( sec_ela));
    
      delay( 1000 );       
}
