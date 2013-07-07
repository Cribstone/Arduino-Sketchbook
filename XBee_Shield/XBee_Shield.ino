//Serial Relay - Arduino will patch a
//serial link between the computer and the Bee Shield
//at 9600 bps 8-N-1
//Computer is connected to Hardware UART
//Bee Shield is connected to the Software UART

#include <SoftwareSerial.h>
#define RxD 11
#define TxD 12

 SoftwareSerial mySerial(RxD,TxD);

void setup()
{
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    Serial.begin(9600);               // the Bee baud rate  
    Serial.begin(9600);                 // the terminal baud rate  
}

void loop()
{
    if(Serial.available())
    {
       Serial.print((unsigned char)Serial.read());
     } 
    else  if(Serial.available())
    {
       Serial.print((unsigned char)Serial.read());
     }  

}
