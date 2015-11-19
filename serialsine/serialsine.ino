#include "math.h"       // This will allow us to run the "sin" function.

void setup() {

  // Enable Serial Output
  Serial.begin(9600);
  while (!Serial);

}

void loop() {

  // Describe the upward swing of the sine wave
  for (double i = -1.0; i <= 1.0; i = i + 0.1) {
    Serial.println (sin(i));
    delay(10);
  }

  // Describe the downward swing of the sine wave
  for (double i = 1.0; i >= -1.0; i = i - 0.1) {
    Serial.println (sin(i));
    delay(10);
  }
  
}
