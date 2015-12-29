
//From bildr article: http://bildr.org/2012/08/rotary-encoder-arduino/
//and from Adafruit’s Larson Scanner https://learn.adafruit.com/larson-scanner-shades

//You will also need Adafruit’s NeoPixel library installed https://github.com/adafruit/Adafruit_NeoPixel

#include <Adafruit_NeoPixel.h>

#define N_LEDS 60

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//these pins can not be changed 2/3 are special pins

int encoderPin1 = 2;

int encoderPin2 = 3;

volatile int lastEncoded = 0;

volatile int encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;

int lastLSB = 0;

void setup() {

strip.begin();

int pos = 1, dir = 1; // Position, direction of "eye"

Serial.begin (9600); // for testing encoder

pinMode(encoderPin1, INPUT);

pinMode(encoderPin2, INPUT);

digitalWrite(encoderPin1, HIGH); //turn pullup resistor on

digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

//call updateEncoder() when any high/low changed seen

//on interrupt 0 (pin 2), or interrupt 1 (pin 3)

attachInterrupt(0, updateEncoder, CHANGE);

attachInterrupt(1, updateEncoder, CHANGE);

}

int pos = 1, dir = 1; // Position, direction of "eye"

void loop(){

int count =0;

count = (encoderValue / 4);

// Serial.println(encoderValue); //testing / debugging encoder

Serial.println(count); //testing / debugging encoder

int j;

long colorb = 0;

long colorg = 0;

long colorr =0;

// int count;

colorb = colorb + abs(encoderValue); // blue

colorg = colorb + 2*abs(encoderValue); //green

colorr = colorb + 4*abs(encoderValue); //red

//lookup table for color values 13 sets of 3 colors

int red [13] = { 255,255,255,255,128,0,0,0,0,0,127,255,255};

int red1[13] = { 128,128,204,204,102,0,0,0,0,0,102,204,204};

int red2[13] = { 32,16,153,153,76,0,0,0,0,0,76,153,153};

int green [13] = {255,0,0,0,0,0,127,255,255,255,255,255,127};

int green1[13] = { 128,0,0,0,0,0,102,64,204,128,204,204,102};

int green2[13] = { 32,0,0,0,0,0,76,32,153,16,153,153,76};

int blue [13] = { 255,0,127,255,255,255,255,255,0,0,0,0,0};

int blue1[13] = { 128,0,102,204,204,128,204,32,102,0,0,0,0};

int blue2[13] = { 32,0,76,153,153,16,153,32,76,0,0,0,0};

if (colorb > 255) colorg = colorg ++;

if (colorg >255) colorr = colorr++;

// Draw 5 pixels centered on pos. setPixelColor() will clip any

// pixels off the ends of the strip, we don't need to watch for that.

strip.setPixelColor(0,(red[count]),(green[count]),(blue[count])); // encoder LED (separate WS2811 chip)

strip.setPixelColor(pos - 2,(red2[count]),(green2[count]),(blue2[count])); // red2 green2 blue2 from lookup table

strip.setPixelColor(pos - 1, (red1[count]),(green1[count]),(blue1[count])); // red1 green1 blue1

strip.setPixelColor(pos , (red[count]),(green[count]),(blue[count])); // Center pixel is brightest

strip.setPixelColor(pos + 1, (red1[count]),(green1[count]),(blue1[count])); // red1 green1 blue1

strip.setPixelColor(pos + 2,(red2[count]),(green2[count]),(blue2[count])); // red2 green2 blue2

strip.show();

delay(30);

// Rather than being sneaky and erasing just the tail pixel,

// it's easier to erase it all and draw a new one next time.

for(j=-2; j<= 2; j++) strip.setPixelColor(pos+j, 0);

// Bounce off ends of strip

pos += dir;

if(pos < 2) {

pos = 3;

dir = -dir;

} else if(pos >= strip.numPixels()) {

pos = strip.numPixels() - 2;

dir = -dir;

}

}

void updateEncoder(){

int MSB = digitalRead(encoderPin1); //MSB = most significant bit

int LSB = digitalRead(encoderPin2); //LSB = least significant bit

int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number

int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;

if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;

if (encoderValue >55) encoderValue =0; // chaNGED 10-26-14

if (encoderValue <0) encoderValue =55;

lastEncoded = encoded; //store this value for next time

}
