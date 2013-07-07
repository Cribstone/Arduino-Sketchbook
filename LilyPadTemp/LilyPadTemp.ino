/* Aly Fielding / @alysonf / http://alysonfielding.com Temperature sensor - flashing and blinking lights
with big thanks to Ladyada's light temp code. See: http://www.ladyada.net/make/logshield/lighttemp.html
*/
 
#define aref_voltage 4.08         // This is a reference voltage for your power supply: as 4.08
//but measure it with a multimeter when running and add in the correct voltage.
 
// 'int' is short for 'integer' = whole number
 
int tempPin = 18;        // the analog pin is connected to temp sensor
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures
int tempReading;        // the analog reading from the sensor
 
int lightPin1 = 5;       //put the LED light on pin 5
int lightPin2 = 6;       //put the LED light on pin 6
int lightPin3 = 9;       //put the LED light on pin 9
int lightPin4 = 10;      //put the LED light on pin 10
int lightPin5 = 11;      //put the LED light on pin 11
 
int groundPin1 = 16;    //setting 16 to be another - (ground) pin
int groundPin2 = 17;    //setting 17 to be another - (ground) pin
int positivePin = 19;   //setting 19 to be a positive pin.
 
int brightness1 = 0;    // how bright the LED is
int fadeAmount1 = 5;    // how many points to fade the LED by
 
int brightness2 = 25;    // how bright the LED is
int fadeAmount2 = 5;    // how many points to fade the LED by
 
int brightness3 = 45;    // how bright the LED is
int fadeAmount3 = 5;    // how many points to fade the LED by
 
int brightness4 = 70;    // how bright the LED is
int fadeAmount4 = 5;    // how many points to fade the LED by
 
int brightness5 = 90;    // how bright the LED is
int fadeAmount5 = 5;    // how many points to fade the LED by
 
void setup(void) {
// To send debugging information via the Serial monitor
Serial.begin(9600);
 
pinMode(lightPin1, OUTPUT); //Setting LED 1 (lightPin1) to be an output. Ditto for each of the other 5 LEDs
pinMode(lightPin2, OUTPUT);
pinMode(lightPin3, OUTPUT);
pinMode(lightPin4, OUTPUT);
pinMode(lightPin5, OUTPUT);
pinMode(groundPin1, OUTPUT);
pinMode(groundPin2, OUTPUT);
pinMode(positivePin, OUTPUT);
digitalWrite(groundPin1, LOW);    //digitalwrite is 'set the value of this pin to be .... LOW = minus , HIGH = positive'
digitalWrite(groundPin2, LOW);    //digitalwrite is 'set the value of this pin to be ....'
digitalWrite(positivePin, HIGH);  //digitalwrite is 'set the value of 19 to be positive
}
 
void loop(void) {
 
tempReading = analogRead(tempPin);  //Get a temperaure reading from the temp sensor
 
Serial.print("Temp reading = "); // allow the temp reading to appear in debugging > Serial Monitor
Serial.print(tempReading);     // the raw analog reading
 
// converting that reading to voltage, which is based off the reference voltage
float voltage = tempReading * aref_voltage;
voltage /= 1024.0;
 
// print out the voltage
Serial.print(" - ");
Serial.print(voltage);
Serial.println(" volts");
 
// now print out the temperature
float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset
//to degrees ((voltage - 500mV) times 100)
Serial.print(temperatureC);
Serial.println(" degrees C");
 
// now convert to Fahrenheight
float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
Serial.print(temperatureF);
Serial.println(" degrees F");
 
if (temperatureC >= 12) { // above 12C the light goes off
 
digitalWrite(lightPin1, LOW);
digitalWrite(lightPin2, LOW);
digitalWrite(lightPin3, LOW);
digitalWrite(lightPin4, LOW);
digitalWrite(lightPin5, LOW);
 
}
 
else if (temperatureC >= 1) { //below 12C and above or equal to 1C, the lights fade in and out
 
// set the brightness of all five LEDS:
analogWrite(lightPin1, max(brightness1, 0));
analogWrite(lightPin2, max(brightness2, 0));
analogWrite(lightPin3, max(brightness3, 0));
analogWrite(lightPin4, max(brightness4, 0));
analogWrite(lightPin5, max(brightness5, 0));
 
// change the brightness for next time through the loop:
brightness1 = brightness1 + fadeAmount1;
brightness2 = brightness2 + fadeAmount2;
brightness3 = brightness3 + fadeAmount3;
brightness4 = brightness4 + fadeAmount4;
brightness5 = brightness5 + fadeAmount5;
 
// reverse the direction of the fading at the ends of the fade:
// Also allow the brightness down to -20 to make the LED fade to black for longer
if (brightness1 == -20 || brightness1 == 100) {
fadeAmount1 = -fadeAmount1;
 
}
 
if (brightness2 == -20 || brightness2 == 100) {
fadeAmount2 = -fadeAmount2;
 
}
 
if (brightness3 == -20 || brightness3 == 100) {
fadeAmount3 = -fadeAmount3;
 
}
 
if (brightness4 == -20 || brightness4 == 100) {
fadeAmount4 = -fadeAmount4;
 
}
 
if (brightness5 == -20 || brightness5 == 100) {
fadeAmount5 = -fadeAmount5;
 
}
 
}
else //If temp drops below 1C, the lights will change status to blink randomly (in sequence)
 
{
 
digitalWrite(lightPin1, HIGH);   // set the LED on
delay(random(100, 400));              // blink randomly
digitalWrite(lightPin1, LOW);    // set the LED off
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin2, HIGH);   // set the LED on
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin2, LOW);    // set the LED off
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin3, HIGH);   // set the LED on
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin3, LOW);    // set the LED off
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin4, HIGH);   // set the LED on
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin4, LOW);    // set the LED off
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin5, HIGH);   // set the LED on
delay(random(100, 400));               // blink randomly
digitalWrite(lightPin5, LOW);    // set the LED off
delay(random(100, 400));               // blink randomly
 
}
 
}

