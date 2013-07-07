/*
  Processing Pong
 
 This sketch input serially from one or two Esploras to play a game of Pong.
 Esploras send four values, separated by commas, and ending with a linefeed:
 slider, switch 1, switch 3, switch 4
 
 The slider sets a paddle's height
 Switch 1 is resets the game
 Switch 2 resets the ball to the center
 Switch 3 reverses the players
 
 The Esplora example EsploraPong will send the correct values to this game.
 
 To start the game, select the Esplora's port number(s). You can play
 with one or two Esploras.
 
 created 22 Dec 2012
 by Tom Igoe
 
 This example is in the public domain.
 */


import processing.serial.*;              // import the Serial library
import java.awt.Rectangle;               // import Java's Rectangle class

Serial[] EsploraList = new Serial[2];    // a list of Serial devices
int portCount = 0;                       // the number of serial ports that's been initialized
String portNumber = "";                  // string of the next port to be initialized

Rectangle leftPaddle, rightPaddle;       // Rectangles for the paddles

int resetButton = 1;              // reset button value
int serveButton = 1;              // serve button value
int switchSidesButton = 1;        // switch sides button value
int paddleHeight = 50;            // vertical dimension of the paddles
int paddleWidth = 10;             // horizontal dimension of the paddles

int ballSize = 10;     // the size of the ball
int xDirection = 2;    // the ball's horizontal direction. left is –2, right is 2.
int yDirection = 2;    // the ball's vertical direction. up is –2, down is 2.
int xPos, yPos;        // the ball's horizontal and vertical positions

boolean ballInMotion = false;  // whether the ball should be moving

int leftScore = 0;      // score for left player
int rightScore = 0;     // score for right player

int fontSize = 20;      // size of the fonts on the screen

void setup() {
  size(640, 480);       // set the size of the applet window

  // initialize the paddles:
  leftPaddle = new Rectangle(50, height/2, paddleWidth, paddleHeight);
  rightPaddle = new Rectangle(width-50, height/2, paddleWidth, paddleHeight);

  // set no borders on drawn shapes:
  noStroke();

  // initialize the ball in the center of the screen:
  xPos = width/2;
  yPos = height/2;

  // create a font with the third font available to the system:
  PFont myFont = createFont(PFont.list()[2], fontSize);
  textFont(myFont);
}


void draw() {
  // clear the screen:
  background(0);
  fill(255);

  // if there's not enough ports initialized:
  if (portCount < 2) {
    // get a list of the serial ports:
    String[] portList = Serial.list();
    // draw instructions on the screen:
    text("Type the port number of Esplora #" + (portCount+1), 20, 20);
    text("(or type enter to finish):", 20, 40);

    // draw the port list on the screen:
    for (int i = 0; i < portList.length; i++) {
      text("port " + i + ":  " + portList[i], 20, (i+4)*20);
    }
  } 
  
  // if you've got all the ports:
  else {
    // draw the left paddle:
    rect(leftPaddle.x, leftPaddle.y, leftPaddle.width, leftPaddle.height);
    // draw the right paddle:
    rect(rightPaddle.x, rightPaddle.y, rightPaddle.width, rightPaddle.height);
    // calculate the ball's position and draw it:
    if (ballInMotion == true) {
      animateBall();
    }

    // print the scores:
    text(leftScore, fontSize, fontSize);
    text(rightScore, width-fontSize, fontSize);
  }
}


// serialEvent  method is run automatically whenever the buffer 
// reaches the byte value set by  bufferUntil():
void serialEvent(Serial thisPort) { 
  // read the serial buffer:
  String inputString = thisPort.readStringUntil('\n');

  // trim the carrige return and linefeed from the input string:
  inputString = trim(inputString);

  // split the input string at the commas
  // and convert the sections into integers:
  int sensors[] = int(split(inputString, ','));

  // if you received all the sensor values, use them:
  if (sensors.length == 4) {
    // if this is the left Esplora:
    if (thisPort == EsploraList[0] && EsploraList[0] != null) {
      // scale the sliders' results to the paddles' range:
      leftPaddle.y = int(map(sensors[0], 0, 1023, 0, height - leftPaddle.height));
    }
    // if this is the right Esplora:
    if (thisPort == EsploraList[1] && EsploraList[1] != null) {
      rightPaddle.y = int(map(sensors[0], 0, 1023, 0, height- rightPaddle.height));
    }
    // if the reset button changes state,
    // reset the score:
    if (resetButton == 1 && sensors[1] == 0) {
      leftScore = 0;
      rightScore = 0;
      resetBall();
      ballInMotion = true;
    }
    // save the current state of the reset button for comparison with the next reading:
    resetButton = sensors[1];

    // if the serve button changes state, reset the ball:
    if (serveButton == 1 && sensors[2] == 0) {
      resetBall();
      ballInMotion = true;
    }
    // save the current state of the serve button for comparison with the next reading:
    serveButton = sensors[2]; 
    
    // if the switch sides button changes state,
    // change the left and right players:
    if (switchSidesButton == 1 && sensors[3] == 0) {
      switchSides();
    }
    // save the current state of the switch sides button for comparison with the next reading:
    switchSidesButton = sensors[3];
  }
}

void animateBall() {
  if (leftPaddle.contains(xPos, yPos) ||    // if the ball pos is inside the left paddle
  rightPaddle.contains(xPos, yPos)) {       // or the ball pos is inside the right paddle
    xDirection = -xDirection;               // reverse the ball's X direction
  }

  // if the ball goes off the screen left:
  if (xPos < 0) {
    rightScore++;
    resetBall();
  }
  // if the ball goes off the screen right:
  if (xPos > width) {
    leftScore++;
    resetBall();
  }


  // stop the ball going off the top or the bottom of the screen:
  if ((yPos <= 0) || (yPos >=height)) {
    // reverse the y direction of the ball:
    yDirection = -yDirection;
  }
  // update the ball position:
  xPos = xPos + xDirection;
  yPos = yPos + yDirection;

  // Draw the ball:
  rect(xPos, yPos, ballSize, ballSize);
}

void resetBall() {
  // put the ball back in the center
  xPos = width/2;
  yPos = height/2;
}



void keyReleased() {
  // if the enter key is pressed, stop looking for port number selections:
  if (key == ENTER) {
    if (portNumber != "" && portCount < 2) {
      choosePort(int(portNumber));
    }
    portCount++;
  }

  // if the user types 0 though 9, use it as a port number selection:
  if (key >= '0' && key <= '9') {
    portNumber += key;
  }
}

void choosePort(int whichPort) {
  // get the port name from the serial list:
  String portName = Serial.list()[whichPort];
  // initialize the next Esplora:
  EsploraList[portCount] = new Serial(this, portName, 9600);
  // read bytes into a buffer until you get a linefeed (ASCII 10):
  EsploraList[portCount].bufferUntil('\n');
  // clear the string holding the port number:
  portNumber = "";
}

void switchSides() {
  Serial temp = EsploraList[0];      // save the first element temporarily
  EsploraList[0] = EsploraList[1];   // move the second elemdnt to the first
  EsploraList[1] = temp;             // put the first element in the second place
}

