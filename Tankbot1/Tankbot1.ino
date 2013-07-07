/*
 * Control Tank motors with TB6612FNG Motor Driver.
 * 
 *    +5V                           +5V
 *     |                             |
 *   R 10K                         R 10K
 *     |                             |
 *    D3   D5   D4   +5V  D8   D7   D9   GND
 *     |    |    |    |    |    |    |    |
 * +---+----+----+----+----+----+----+----+---+
 * |   A    A    A    S    B    B    B    D   |
 * |   M    l    l    T    l    l    M    N   |
 * |   W    n    n    B    n    n    W    G   |
 * |   P    1    2    Y    1    2    P        |
 * |                                          |
 * |             TB6612FNG                    |
 * |                                          |
 * |             1    2    1    2             |
 * |             t    t    t    t    T        |
 * |   D    C    u    u    u    u    O    D   |
 * |   N    C    O    O    O    O    M    N   |
 * |   G    V    A    A    B    B    V    G   |
 * +---+----+----+----+----+----+----+----+---+
 *     |    |    |    |    |    |    |    | 
 *     |    |    |    |    |    |    |    |
 *    GND  +5V  (+)  (-)  (+)  (-)  +9V  GND
 *              Motor A   Motor B   Battery
 *
 *  Use Serial port to send commands to Arduino and move the tank.
 */

// Motor A
#define leftSpeedPin    3   // Left motor speed control, to PWM_A
#define leftDir1        5   // Left motor direction 1, to AIn1
#define leftDir2        4   // Left motor direction 2, to AIn2

// Motor B
#define rightDir1        8  // Right motor direction 1, to BIn1
#define rightDir2        7 // Right motor direction 2, to BIn2
#define rightSpeedPin    9  // Right motor speed control, to PWM_B 

// LEDs
#define lfLED 15
#define rfLED 14


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
  pinMode(lfLED, OUTPUT);
  pinMode(rfLED, OUTPUT);  

  // Serial 
  Serial.begin(9600);
  Serial.println("Robot ready for mission:");
  Serial.println("Commands: w, s, a, d, q, e");
  
  // Turn the light on.
  digitalWrite(lfLED, HIGH);
  digitalWrite(rfLED, HIGH); 
  
} 

// Loop all day long...

void loop()  
{  
  // Listen for incoming messages 
  if (Serial.available() > 0 )  {
  
    char command = Serial.read();
    
    if (command == 'w' || command == 'a' || command == 'd') {
      forward(command,200,2000);    
    }
  
    if (command == 's' || command == 'q' || command == 'e') {
      backward(command,200,2000);
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
 * Function: Rotate the robot
 * @param int dec 
 *
 *        (360)
 *   (315)  |  (45)
 *  (270) - x - (90)
 *   (225)  |  (135)
 *        (180)
 */

void rotate(int dec) {
  
  switch ( dec ) {
    case 360:
      rotateLeft(2500);
      break;
    case 45:
     rotateLeft(500);
     break;
    case 90:
      rotateLeft(800);
      break;
    case 135:
      rotateLeft(1200);
      break;
    case 180:
      rotateLeft(1300);
      break;
    case 255:
      rotateLeft(1600);
      break;
    case 270:
      rotateLeft(1800);
      break;
    case 315:
      rotateLeft(2000);
      break;
    default:
      // do nothing
      break;
  }
  
}

/*
 * Function: Rotate robot left 
 * @param: int x 
 *
 */

void rotateLeft(int x) {
  
  // Left side go forward
  digitalWrite(leftSpeedPin, HIGH);
  digitalWrite(leftDir1, HIGH);    
  digitalWrite(leftDir2, LOW);    
  
  // Right side go backward
  digitalWrite(rightSpeedPin, HIGH);
  digitalWrite(rightDir1, LOW);    
  digitalWrite(rightDir2, HIGH);    
  
  delay(x);
  
  // Stop all motors and wait for next command.
  digitalWrite(leftSpeedPin, LOW);
  digitalWrite(leftDir1, LOW);    
  digitalWrite(rightSpeedPin, LOW);
  digitalWrite(rightDir2, LOW);

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
      delay(10); // Delay 
    } 

    delay(distance/4); 

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
      delay(10);
    }    

    delay(distance/4);

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
      delay(10);
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
      delay(10);
    } 

    delay(distance/4);  

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
