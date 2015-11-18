/*
  Visualight Blink
  
  Also see: 
  https://github.com/lpercifield/visualight/blob/master/Firmware/Visualight/examples/visualight_LED/visualight_LED.ino
  https://github.com/lpercifield/visualight
  http://visualight.org
  
  The Visualight can be programmed using the Arduino IDE, selecting "Arduino Leonardo" under "Boards". 

 */
 
// Pins 9, 13, and 10 are the red, green and blue LEDs on the Visualight board.
// give them names:
int red = 9;
int green = 13;
int blue = 10;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(red, OUTPUT);     
  pinMode(green, OUTPUT);     
  pinMode(blue, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(red, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(red, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  digitalWrite(green, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(green, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  digitalWrite(blue, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(blue, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
