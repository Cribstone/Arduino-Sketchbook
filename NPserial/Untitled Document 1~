#include <Adafruit_NeoPixel.h>

#define PIN 4
int led = 13;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);


uint16_t n;
uint8_t r;
uint8_t g;
uint8_t b;
String content = "";
char character;
byte count;
int l = 0;

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
                
// Turn the Serial Protocol ON
  Serial.begin(57600);
    colorWipe(strip.Color(0, 0, 0), 10); // Red
}

void loop() {
  // Check for data arriving
  if (Serial.available()) {
    character = Serial.read();
    if ((count == 6 ) && (character == 'N')) {
      count = 0;
      content="";
    }
    if (count < 5) {
      content.concat(character);
      count += 1;
    }
    if (count == 5) {
      digitalWrite(led, HIGH);
      Serial.print("rgb ");
      n = content[1];
      r = content[2];
      g = content[3];
      b = content[4];
      n = n%8;
      Serial.println(n);
      //Serial.print(" - ");
      //Serial.println(content);
      strip.setPixelColor(n, strip.Color(r, g, b));
      strip.show();
      count = 6;
      digitalWrite(led, LOW);
    }
  }
  else {
    delay(25);
  }
  l++;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

String defloat(float f) {
  int a = (int) f;
  int b = (int) ((f - a) * 100);
  String s = "";
  s.concat(a);
  s.concat(".");
  s.concat(b);
  return s;
}
