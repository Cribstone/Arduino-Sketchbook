// Motor A
#define leftSpeedPin    16   // Left motor speed control, to PWM_A
#define leftDir1        14   // Left motor direction 1, to AIn1
#define leftDir2        15   // Left motor direction 2, to AIn2

// Motor B
#define rightDir1        17  // Right motor direction 1, to BIn1
#define rightDir2        18 // Right motor direction 2, to BIn2
#define rightSpeedPin    19  // Right motor speed control, to PWM_B 

// LEDs
#define leftLED 2
#define rightLED 3

byte leftLEDState = HIGH;
byte rightLEDState = HIGH;
// setup

void setup()
{
  int motorSpeed;                   // From 0->255, slow->fast.

  // Set motor direction pins as outputs.
  pinMode(leftDir1, OUTPUT);
  pinMode(leftDir2, OUTPUT);
  pinMode(rightDir1, OUTPUT);
  pinMode(rightDir2, OUTPUT);

  // LEDs
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);

  digitalWrite(leftLED, leftLEDState);   // sets the LED on
  digitalWrite(rightLED, rightLEDState);    // sets the LED off

  // Serial 
  Serial.begin(9600);
  Serial.println("Robot ready for mission:");
  Serial.println("Commands: w, s, a, d, j, k");

}

// Loop all day long...

void loop()
{

  // Listen for incoming messages 
  if (Serial.available() > 0 )  {

    char command = Serial.read();

    if (command == 'w' || command == 'a' || command == 'd') {
      forward(command,200,0);
    }

    if (command == 's') {
      backward(command,200,0);
    }

    if (command == 'j' || command == 'k') {
      frontLight(command);
    }

    // Debug
    Serial.println(command);
  }
}

/* *********************************************************
 *
 * Functions 
 *
 * *********************************************************/

/*
 * Function: Control front light LED
 * @param: char led
 */
void frontLight(char led) {
  // Left led
  if (led == 'j') {
    if (leftLEDState == LOW) {
      leftLEDState = HIGH;

    } else {
      leftLEDState = LOW;
    }
    digitalWrite(leftLED, leftLEDState);
  }
  // Right led
  if (led == 'k') {
    if(rightLEDState == LOW) {
      rightLEDState = HIGH;
    } else {
      rightLEDState = LOW;
    }
    digitalWrite(rightLED,rightLEDState);
  }

}
/*
 * Function: Move the robot forward
 * @param: char motor
 * @param: int motorSpeed
 * @param: int distance
 *
 */

void forward(char motor, int motorSpeed, int distance) {

  // Counter for the speed.
  int current;

  // Turn the right motor forward.
  if (motor == 'd') {

    digitalWrite(leftSpeedPin, HIGH);
    digitalWrite(leftDir1, HIGH);
    digitalWrite(leftDir2, LOW);

    // Move the motor step by step.
    for (current = 0; current <= motorSpeed; current++)
    {
      analogWrite(leftSpeedPin, current);
      delay(6); // Delay 
    }

    delay(distance);

    digitalWrite(leftSpeedPin, LOW);
    digitalWrite(leftDir1, LOW);

  }

  // Turn the left motoro forward.
  if (motor == 'a') {

    digitalWrite(rightSpeedPin, HIGH);
    digitalWrite(rightDir1, HIGH);
    digitalWrite(rightDir2, LOW);

    for (current = 0; current <= motorSpeed; current++)
    {
      analogWrite(rightSpeedPin, current);
      delay(6);
    }

    delay(distance);

    digitalWrite(rightSpeedPin, LOW);
    digitalWrite(rightDir1, LOW);

  }

  // Turn left and right motor forward.
  if (motor == 'w') {

    digitalWrite(leftSpeedPin, HIGH);
    digitalWrite(leftDir1, HIGH);
    digitalWrite(leftDir2, LOW);

    digitalWrite(rightSpeedPin, HIGH);
    digitalWrite(rightDir1, HIGH);
    digitalWrite(rightDir2, LOW);


    for (current = 0; current <= 255; current++)
    {
      analogWrite(leftSpeedPin, current);
      analogWrite(rightSpeedPin, current);
      delay(7);
    }
    delay(distance);

    digitalWrite(leftSpeedPin, LOW);
    digitalWrite(leftDir1, LOW);

    digitalWrite(rightSpeedPin, LOW);
    digitalWrite(rightDir1, LOW);

  }

}

/*
 * Function: Move the robot backward
 * @param: char motor
 * @param: int motorSpeed
 * @param: int distance
 *
 */

void backward(char motor, int motorSpeed, int distance) {

  int current;

  // Move the left motor backward
  if (motor == 'e') {

    digitalWrite(leftSpeedPin, HIGH);
    digitalWrite(leftDir1, LOW);
    digitalWrite(leftDir2, HIGH);

    for (current = 0; current <= motorSpeed; current++)
    {
      analogWrite(leftSpeedPin, current);
      delay(5);
    }

    delay(distance);

    digitalWrite(leftSpeedPin, LOW);
    digitalWrite(leftDir2, LOW);

  }

  // Move the right motor backward.
  if (motor == 'q') {

    digitalWrite(rightSpeedPin, HIGH);
    digitalWrite(rightDir1, LOW);
    digitalWrite(rightDir2, HIGH);

    for (current = 0; current <= motorSpeed; current++)
    {
      analogWrite(rightSpeedPin, current);
      delay(10);
    }

    delay(distance/4);

    digitalWrite(rightSpeedPin, LOW);
    digitalWrite(rightDir2, LOW);

  }

  // Move the left and right motor backward.
  if (motor == 's') {

    digitalWrite(leftSpeedPin, HIGH);
    digitalWrite(leftDir1, LOW);
    digitalWrite(leftDir2, HIGH);

    digitalWrite(rightSpeedPin, HIGH);
    digitalWrite(rightDir1, LOW);
    digitalWrite(rightDir2, HIGH);

    for (current = 0; current <= motorSpeed; current++)
    {
      analogWrite(leftSpeedPin, current);
      analogWrite(rightSpeedPin, current);
      delay(20);
    }
    delay(distance);

    digitalWrite(leftSpeedPin, LOW);
    digitalWrite(leftDir2, LOW);

    digitalWrite(rightSpeedPin, LOW);
    digitalWrite(rightDir2, LOW);

  }

}

