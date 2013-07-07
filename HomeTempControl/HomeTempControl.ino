#include <OneWire.h>
#include <DallasTemperature.h>
#define TEMPERATURE_PRECISION 12
#define ONE_WIRE_BUS 3 // Onewire data wire is plugged into port 2 on the Arduino. Needs ca 4k pullup resistor to 5V.

OneWire oneWire(ONE_WIRE_BUS); // Setup 1Wire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
DeviceAddress insideThermometer, outsideThermometer, thirdThermometer; // arrays to hold device addresses
int d=314;  // delay adjusted to macth original wireless remote.  Use a digital oscilloscope to check.  
int tpin=10;  // 433 MHz RF transmitter hooked to this pin (soldered parallel to the remote encoding chip output.)
int dig1pin=11; int dig2pin=12; // digital outputs that can be switched on with a serial command
String dig1="d1off"; String dig2="d2off";
String tA1=" A1?";  String tA2="A2?"; String tA3="A3?"; // last command output to wireless socket
int count=0; //byte count in serial message
int ca0=0, cb0=0, cc0=0, state = LOW;

void setup() {
  pinMode(tpin, INPUT); // we unblock transmit pin when not in use so that normal remote functions too.     
  pinMode(dig1pin, OUTPUT);
  pinMode(dig2pin, OUTPUT);
  pinMode(13, OUTPUT);
  attachInterrupt(0, stateChange, CHANGE);
  
  Serial.begin(9600); Serial.flush();
  Serial.println ("Reads AI0, AI1, AI2. Controls digital output pins 11, 12. Commands to switch digital outputs: D1on, D1off, D2on, D2off.");
  Serial.println ("Generates 1-wire bus (pin3) for DS18B20 temperature sensors. Needs external4.7k pullup.");
  Serial.println ("433 MHz wireless power outlet chip HS2260A-R4 simulation (pin 10). Commands: A1on, A1off, A2on, A2off, A3on, A3off.");
  Serial.println ("counter using interrupt on pin 2. Good for Geiger counter click counting, output to pin13"); 
  Serial.println ("Reset button of Arduino is disabled to 5V, remove jumper for reprogramming");
  Serial.println ("Make sure there is no echo from USB host back to Arduino, buffer will overload and commanding will not work");
  Serial.println ("stty -F /dev/ttyACM0  speed 9600 -echo");

   sensors.begin();
    if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find  1-wire sensor nr 1"); 
    if (!sensors.getAddress(outsideThermometer, 1)) Serial.println("Unable to find 1-wire sensor nr 2"); 
    if (!sensors.getAddress(thirdThermometer, 2)) Serial.println("Unable to find 1-wire sensor nr 3"); 
    sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
    sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
    sensors.setResolution(thirdThermometer, TEMPERATURE_PRECISION);

}
void stateChange(){state=!state; ca0++; digitalWrite(13, state);}

// functions for wireless remote control emulation
  void I() {     // sends one
           digitalWrite(tpin, HIGH); delayMicroseconds(3*d);   digitalWrite(tpin, LOW);  delayMicroseconds(1*d); 
           digitalWrite(tpin, HIGH); delayMicroseconds(3*d);   digitalWrite(tpin, LOW);  delayMicroseconds(1*d); }
  void O() {    // sends zero
           digitalWrite(tpin, HIGH); delayMicroseconds(1*d);   digitalWrite(tpin, LOW);  delayMicroseconds(3*d); 
           digitalWrite(tpin, HIGH); delayMicroseconds(1*d);   digitalWrite(tpin, LOW);  delayMicroseconds(3*d); }
  void F() {   // sends float     
           digitalWrite(tpin, HIGH); delayMicroseconds(1*d);  digitalWrite(tpin, LOW);  delayMicroseconds(3*d); 
           digitalWrite(tpin, HIGH); delayMicroseconds(3*d);  digitalWrite(tpin, LOW);  delayMicroseconds(1*d); }
  void E()  {   // synchronization
          digitalWrite(tpin, HIGH);  delayMicroseconds(1*d);  digitalWrite(tpin, LOW);   delayMicroseconds(31*d);  }

// functions for 1wire bus
void printAddress(DeviceAddress deviceAddress) { for (uint8_t i = 0; i < 8; i++)  {  }  }
void printTemperature(DeviceAddress deviceAddress) { float tempC = sensors.getTempC(deviceAddress); Serial.print(tempC); Serial.print(" ");  }
void printResolution(DeviceAddress deviceAddress) {  }
void printData(DeviceAddress deviceAddress) {  printAddress(deviceAddress);  printTemperature(deviceAddress); }

void loop() {
String kommand=""; // string received via com port
count=Serial.available();
 for (int x=0; x<count; x++) { char ch = Serial.read(); kommand = kommand + ch;}
//if (kommand != "") {Serial.print (kommand); Serial.println (count);}
      kommand=kommand.substring(0,4);
  if (kommand=="A1on") {tA1=" A1on";   pinMode(tpin, OUTPUT);     
     for (int i=0; i <= 20; i++) { I();F();F();F();I();F();F();O();O();O();I();O();E(); }    }

  if (kommand=="A1of") {tA1=" A1off"; pinMode(tpin, OUTPUT);
     for (int i=0; i <= 20; i++) { I();F();F();F();I();F();F();O();O();O();O();I();E(); }    }

  if (kommand=="A2on") {tA2="A2on"; pinMode(tpin, OUTPUT);
     for (int i=0; i <= 20; i++) { I();F();F();F();F();I();F();O();O();O();I();O();E(); }    }

  if (kommand=="A2of") {tA2="A2off"; pinMode(tpin, OUTPUT);
     for (int i=0; i <= 20; i++) { I();F();F();F();F();I();F();O();O();O();O();I();E(); }    }

  if (kommand=="A3on") {tA3="A3on"; pinMode(tpin, OUTPUT);
     for (int i=0; i <= 20; i++) { I();F();F();F();F();F();I();O();O();O();I();O();E(); }    }

  if (kommand=="A3of") {tA3="A3off"; pinMode(tpin, OUTPUT);
     for (int i=0; i <= 20; i++) { I();F();F();F();F();F();I();O();O();O();O();I();E(); }     }

  if (kommand=="d1on") {dig1="d1on"; digitalWrite(dig1pin, 1);}
  if (kommand=="d1xof") {dig1="d1off"; digitalWrite(dig1pin, 0);}
  if (kommand=="d2on") {dig2="d2on"; digitalWrite(dig2pin, 1);}
  if (kommand=="d2of") {dig2="d2off"; digitalWrite(dig2pin, 0);}
pinMode(tpin, INPUT);
Serial.print("databegin ");
sensors.requestTemperatures();  printData(insideThermometer);  printData(outsideThermometer);  printData(thirdThermometer);
if (ca0 != cb0){cc0++;cb0=ca0;}
Serial.print(analogRead(A0));
Serial.print(" ");
Serial.print(analogRead(A1));
Serial.print(" ");
Serial.print(analogRead(A2));
Serial.print(" ");
Serial.print(tA1+" "+tA2+" "+tA3+" ");
Serial.print(dig1+" "+dig2+" ");
Serial.print(String(cc0));
Serial.print(" dataend ");
Serial.println(" ");
delay(500);  
}
