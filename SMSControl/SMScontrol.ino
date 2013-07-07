#include <SoftwareSerial.h>
 

// EN: String buffer for the GPRS shield message
String msg = String("");
// EN: Set to 1 when the next GPRS shield message will contains the SMS message
int SmsContentFlag = 0;
//control pins of relay.
int relay_a=4;
int relay_b=5;
int relay_c=6;
int relay_d=7;

// EN: Code PIN of the SIM card (if applied)
//String SIM_PIN_CODE = String( "XXXX" );
 
void setup()
{
  Serial.begin(19200);                 // the GPRS baud rate
  // Initialize  PINs
  pinMode( 4, OUTPUT ); 
  pinMode( 5, OUTPUT ); 
  pinMode( 6, OUTPUT ); 
  pinMode( 7, OUTPUT ); 
  digitalWrite( 4, LOW ); 
  digitalWrite( 5, LOW ); 
  digitalWrite( 6, LOW );
  digitalWrite( 7, LOW );
 Serial.println( "AT+CMGF=1" ); 
 delay(200);
}
 
void loop()
{
    char SerialInByte;
    if(Serial.available())
    {       
        SerialInByte = (unsigned char)Serial.read();
       delay(5);
        
        // -------------------------------------------------------------------
        // EN: Program also listen to the GPRS shield message.
        // -------------------------------------------------------------------
       // EN: If the message ends with <CR> then process the message
        if( SerialInByte == 13 ){
          // EN: Store the char into the message buffer
          ProcessGprsMsg();
         }
         if( SerialInByte == 10 ){
            // EN: Skip Line feed
         }
         else {
           // EN: store the current character in the message string buffer
           msg += String(SerialInByte);
         }
     }   
}
// EN: Make action based on the content of the SMS. 
//     Notice than SMS content is the result of the processing of several GPRS shield messages.
void ProcessSms( String sms ){
  
  if( sms.indexOf("ona") >= 0 ){
    digitalWrite( relay_a, HIGH );
  }
   if( sms.indexOf("onb") >= 0 ){
    digitalWrite(  relay_b, HIGH );
  }
   if( sms.indexOf("onc") >= 0 ){
    digitalWrite(  relay_c, HIGH );
  }
  if( sms.indexOf("ond") >= 0 ){
    digitalWrite(  relay_d, HIGH );
  }
  if( sms.indexOf("offa") >= 0 ){
    digitalWrite(  relay_a, LOW );
  }
  if( sms.indexOf("offb") >= 0 ){
    digitalWrite(  relay_b, LOW );
  }
  if( sms.indexOf("offc") >= 0 ){
    digitalWrite(  relay_c, LOW );
  }
  if( sms.indexOf("offd") >= 0 ){
    digitalWrite(  relay_d, LOW );
  }
}
// EN: Request Text Mode for SMS messaging
void GprsTextModeSMS(){
  Serial.println( "AT+CMGF=1" );
}

void GprsReadSmsStore( String SmsStorePos ){
  Serial.print( "AT+CMGR=" );
  Serial.println( SmsStorePos );
}

// EN: Clear the GPRS shield message buffer
void ClearGprsMsg(){
  msg = "";
}

// EN: interpret the GPRS shield message and act appropiately
void ProcessGprsMsg() {
  if( msg.indexOf( "Call Ready" ) >= 0 ){
   //  Serial.println( "*** GPRS Shield registered on Mobile Network ***" );
     GprsTextModeSMS();
  }
  
  // EN: unsolicited message received when getting a SMS message
  if( msg.indexOf( "+CMTI" ) >= 0 ){
   //  Serial.println( "*** SMS Received ***" );
     // EN: Look for the coma in the full message (+CMTI: "SM",6)
     //     In the sample, the SMS is stored at position 6
     int iPos = msg.indexOf( "," );
     String SmsStorePos = msg.substring( iPos+1 );
   //  Serial.print( "SMS stored at " );
  //   Serial.println( SmsStorePos );     
     // EN: Ask to read the SMS store
     GprsReadSmsStore( SmsStorePos );
  }
  
  // EN: SMS store readed through UART (result of GprsReadSmsStore request)  
  if( msg.indexOf( "+CMGR:" ) >= 0 ){
    // EN: Next message will contains the BODY of SMS
    SmsContentFlag = 1;
    // EN: Following lines are essentiel to not clear the flag!
    ClearGprsMsg();
    return;
  }
  
  // EN: +CMGR message just before indicate that the following GRPS Shield message 
  //     (this message) will contains the SMS body 
  if( SmsContentFlag == 1 ){
 //   Serial.println( "*** SMS MESSAGE CONTENT ***" );
 //   Serial.println( msg );
 //   Serial.println( "*** END OF SMS MESSAGE ***" );
    ProcessSms( msg );
  }
  
  ClearGprsMsg();
  // EN: Always clear the flag
  SmsContentFlag = 0; 
}


