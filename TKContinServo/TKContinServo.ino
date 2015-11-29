
#include <Servo.h>

Servo oneServo;

void setup() {
  oneServo.attach(5);
}

void loop() {
  oneServo.write(360);

}
