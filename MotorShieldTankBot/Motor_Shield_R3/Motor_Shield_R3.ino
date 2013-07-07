//PWM control for motor outputs 1 and 2 is on digital pin 3
int pwm_a = 3;
//PWM control for motor outputs 3 and 4 is on digital pin 11
int pwm_b = 11;

//direction control for motor outputs 1 and 2 is on digital pin 12
int dir_a = 12;
//direction control for motor outputs 3 and 4 is on digital pin 13
int dir_b = 13;

// break control for motor output 1 and 2 is on digital pin 9
int brk_a = 9;
// break control for motor output 3 and 4 is on digital pin 8
int brk_b = 8;

void setup()
{
  pinMode(pwm_a, OUTPUT);
  pinMode(pwm_b, OUTPUT);
  pinMode(dir_a, OUTPUT);
  pinMode(dir_b, OUTPUT);

  pinMode(brk_a, OUTPUT);
  pinMode(brk_b, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  // First set the direction (in my setup FWD is LOW and BCK is HIGH)
  digitalWrite(dir_a, LOW);  //Set motor direction, 1 low, 2 high
  digitalWrite(dir_b, LOW);  //Set motor direction, 3 high, 4 low

  Serial.println("Direction Fwd");

  //set both motors to run at 100% duty cycle (fast)
  analogWrite(pwm_a, 255);
  analogWrite(pwm_b, 255);

  Serial.println("Full Speed");

  // We use a delay to define a period for the motors to run
  delay(2000);

  // Apply the breaks
  digitalWrite(brk_a, HIGH);
  digitalWrite(brk_b, HIGH);

  Serial.println("Break On");

  // Change the direction (in my setup FWD is LOW and BCK is HIGH)
  digitalWrite(dir_a, HIGH);  //Reverse motor direction, 1 high, 2 low
  digitalWrite(dir_b, HIGH);  //Reverse motor direction, 3 low, 4 high

  // Remove the breaks
  digitalWrite(brk_a, LOW);
  digitalWrite(brk_b, LOW);

  Serial.println("Break Off");
  Serial.println("Direction Bck");

  // Again use a delay to define a period for the motors to run
  delay(2000);
  
  // Turn
  // digitalWrite(dir_a, HIGH);
  // digitalWrite(dir_b, LOW);
  
  // delay(2000);
  
  // Turn
  // digitalWrite(dir_a, LOW);
  // digitalWrite(dir_b, HIGH);
  
  // delay(2000);
}
