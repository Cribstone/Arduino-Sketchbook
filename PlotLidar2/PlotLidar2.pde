// Code to plot a depth scan map from Arduino and LidarLite...
// Dincer Hepguler 2015
// http://www.borsaci06.com/

 import processing.serial.*;
 
 float pcolor = 0;        // point color value
 float xVal = 0;      // x  value
 float yVal = 0;      // y  value
 
 Serial myPort;
 
 void setup() {
 size(400, 200);
 
 // List all the available serial ports
 println(Serial.list());
 // I know that the first port in the serial list on my mac
 // is always my  Arduino, so I open Serial.list()[0].
 // Open whatever port is the one you're using.
 myPort = new Serial(this, Serial.list()[1], 57600);
 // don't generate a serialEvent() unless you get a newline character:
 myPort.bufferUntil('\n');
 background(0);
 }
 
 void draw() {
 // set the point color with the color values:
 stroke(pcolor);
 point(xVal,yVal);
 }
 
 void serialEvent(Serial myPort) { 
 // get the ASCII string:
 String inString = myPort.readStringUntil('\n');
 
 if (inString != null) {
 // trim off any whitespace:
 inString = trim(inString);
 // split the string on the commas and convert the 
 // resulting substrings into an integer array:
 float[] colors = float(split(inString, ","));
 // if the array has at least three elements, you know
 // you got the whole thing.  Put the numbers in the
 // color variables:
 if (colors.length >=3) {
 // map them to the range 0-255:
 xVal = colors[0];
 yVal = colors[1];
 pcolor = map(colors[2], 0, 400, 255, 0);
 
 }
 }
 }