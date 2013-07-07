//Code from Sparkfun with a few tweeks to get it working with Rover.  Rather than serial printing 
//words, I switched it to print the letters that the rover sketch understands. 


const byte PIN_ANALOG_X = 0;
const byte PIN_ANALOG_Y = 1;

const int X_THRESHOLD_LOW = 450;   //I have the contrains real big so there's less chance for 
const int X_THRESHOLD_HIGH = 550;  //analog misinterpretation. 

const int Y_THRESHOLD_LOW = 450;
const int Y_THRESHOLD_HIGH = 550;

int x_position;
int y_position;

int x_direction;
int y_direction;

void setup() {
Serial.begin(9600);
}

void loop () {
x_direction = 0;
y_direction = 0;

x_position = analogRead(PIN_ANALOG_X);
y_position = analogRead(PIN_ANALOG_Y);


if (x_position > X_THRESHOLD_HIGH) {
x_direction = 1;
} else if (x_position < X_THRESHOLD_LOW) {
x_direction = -1;
}

if (y_position > Y_THRESHOLD_HIGH) {
y_direction = 1;
} else if (y_position < Y_THRESHOLD_LOW) {
y_direction = -1;
}


if (x_direction == -1) {
if (y_direction == -1) {
Serial.println("left-down");
} else if (y_direction == 0) {
Serial.println("a");                               //the A is whats making it go left
} else {
// y_direction == 1
Serial.println("left-up");
}
} else if (x_direction == 0) {
if (y_direction == -1) {
Serial.println("s");                          // S is for reverse
} else if (y_direction == 0) {
Serial.println("f");                          // F is for Stop--it sends this until something else is sent
} else {
// y_direction == 1
Serial.println("w");                       // W is forward
}
} else {
// x_direction == 1
if (y_direction == -1) {
Serial.println("right-down");
} else if (y_direction == 0) {
Serial.println("d");                         // D is right
} else {
// y_direction == 1
Serial.println("right-up");
}
}}
   
