#include <Usb.h>

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
  uint8_t rcode;
  uint8_t msg[64] = { 0x00 };
  const char* recv = "Received: "; 
   
   accessory.refresh();
   if(!accessory.isConnected()) {
     return;
   }
   
   uint16_t len = 0;

  while (accessory.available() > 0) {
    msg[len] = accessory.read();
    len++;
  }
   
   if(len > 0) {
     USBTRACE("\r\nData Packet.");

    for( uint8_t i = 0; i < len; i++ ) {
      Serial.print((char)msg[i]);
    }
    /* sending back what was received */    
    rcode = accessory.write( (uint8_t *)recv, strlen( recv ) );    
    rcode = accessory.write( msg, strlen(( char * )msg ) );

   }//if( len > 0 )...

   delay( 1000 );       
}

