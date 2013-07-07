Arduino 1.0 BluetoothShieldDemoCode for BluetoothShield http://www.seeedstudio.com/depot/bluetooth-shield-p-866.html?cPath=132_134
//----------------------------------------------------
In Master.ino and Slave.ino,change the line "#include <NewSoftSerial.h>" to #include <SoftwareSerial.h> and change "NewSoftSerial" to "SoftwareSerial".

Note:For SoftwareSerial to be able to receive serial data somewhat asynchronously, a receive pin needs to be able to trigger an external interrupt.So pin2..pin7 on Mega board should not be defined as sofeware serial port.

by Frankie at Seeedstudio.