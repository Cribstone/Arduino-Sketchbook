/*
  R2-Key2 Piano by Team 'Duino from GWCintuit Class of 2014
 When user touches one of the paper keys, Arduino identifies the number of key being pressed and transfers the data to Processing.
 After receiving data, the code below allows Processing to show one of the four visualizations depending on the key and the time it's being pressed.
 
 The four visualizations are:
 1. drawing circles with corresponding color at random spots
 2. drawing bars with corresponding color and length depending on how long the same key is being pressed
 3. drawing a continuous spiral with corresponding colors
 4. using a recursive method to draw circles with their size depending on how long the same key is being pressed
 
 Project modified from the "Capacitive-Touch Arduino Keyboard Piano" originally posted on Instructables.com
 Version Aug 1, 2014
 */

// for data transfer between Arduino and Processing
import processing.serial.*;
Serial myPort;  // Create object from Serial class
int preData; // data previously received
int dataReceived; // data currently received

boolean pause;

//dealing with drawing rectangles
ArrayList<Integer> xPos;
final int yPos = 0;
ArrayList<Float> heightRect;
final int widthRect = 7;
final int speed = 10;
final int canvasWidth = 1438;
final int canvasHeight = 770;
final int minHeight = 100; //minimum height of rects

//storing the whatever key pressed
//will be used to match the corresponding color of each rect
ArrayList<Integer> pianoKey;

//formula: incrementation * count = new max height
final int incrementation = 12; 
int count = 0; //number of rects or how long the key is being pressed

//data visualization options
int dataViz = 1; //default = drawing circles

//for drawing spiral
float r = 0;
float theta = 0;

//for drawing recursive circles
float x;
float y;

//-----------------------------------------------------------------


void setup()
{
  //initialization
  //frame.setResizable(true);
  size(canvasWidth, canvasHeight);
  xPos = new ArrayList<Integer>();
  heightRect = new ArrayList<Float>();
  background(255, 255, 255);
  noStroke();

  pianoKey = new ArrayList<Integer>();

  //Serial port and data transfer
  String portName = Serial.list()[7]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
  preData = 0;
  //dataReceived = 1;

  //draw pause and switch buttons
  pauseButton();
  switchButton();
}



void draw()
{
  if ( myPort.available() > 0) 
  {  // If data is available,
    dataReceived = myPort.read(); // read it and store it in dataReceived
    println(dataReceived); //print it out in the console, for debugging

    if (dataReceived != 9 && dataReceived != 0) 
    {
      if (dataViz == 1)
      {
        draw_circles();
      } else if (dataViz == 2)
      {
        draw_musical_lines();
      } else if (dataViz == 3)
      {
        draw_spiral();
      } else if (dataViz == 4)
      {
        x = random ( 0, canvasWidth);
        y= random (0, canvasHeight);

        if (dataReceived == preData)
        {
          count++;
        } else {
          count = 0;
        }

        change_color();
        makeCircles(count);
        preData = dataReceived;
      }
    } 

    //not working why?????
    if (dataReceived == 9) {
      preData = dataReceived;
    } //end if data != 0
  }//end if myPort.available() > 0

  //draws the pause and switch buttons on top of everything
  pauseButton();
  switchButton();
}//end draw()



/*
    data visualization option 1
 */
void draw_circles()
{
  stroke(0);
  strokeWeight(1);

  //choosing the corresponding color
  change_color();

  //random position of circles
  float x = random(0, canvasWidth);
  float y = random(0, canvasHeight);
  float diameter = random(10, 70);
  ellipse(x, y, diameter, diameter);
}



/*
    data visualization option 2
 */
void draw_musical_lines()
{
  if (preData == dataReceived) {
    /*
     Checks if finger still touching the same key
     If so, current data reading should be the same as the previous one
     Thus increase count by 1, 
     which will cause max height range of rects to increase later
     */
    count++;
  } else {
    //no longer touching key, therefore reset count
    count = 0;
  }

  background(255, 255, 255);
  noStroke();

  //adding new rect
  xPos.add(canvasWidth);
  //increasing height for new rects  
  float h = random(minHeight, height_incrementation());
  heightRect.add(h);

  //helps to identify color of each rect 
  //depending on what key they belonged to
  pianoKey.add(dataReceived);

  //redrawing each rectangle
  for ( int i = 0; i < xPos.size (); i++)
  {
    //setting color of each rect depending on what key they belonged to
    if (pianoKey.get(i) == 1)
    {
      fill(230, 90, 146); //pink
    } else if (pianoKey.get(i) == 2)
    {
      fill(242, 56, 56); //red
    } else if (pianoKey.get(i) == 3)
    {
      fill(237, 126, 71); //orange
    } else if (pianoKey.get(i) == 4)
    {
      fill(250, 250, 105); //yellow
    } else if (pianoKey.get(i) == 5)
    {
      fill(103, 152, 97); //green
    } else if (pianoKey.get(i) == 6)
    {
      fill(64, 224, 208); //turquoise
    } else if (pianoKey.get(i) == 7)
    {
      fill(55, 152, 237); //blue
    } else if (pianoKey.get(i) == 8)
    {
      fill(141, 0, 207); //purple
    }

    rect(xPos.get(i), yPos, widthRect, heightRect.get(i));
    //reset the xPos of each rect to their current xPos minus speed
    xPos.set(i, xPos.get(i) - speed);

    //remove ones that are out of bounds depending on their xPos
    if (xPos.get(i) < 0)
    {
      xPos.remove(i);
      heightRect.remove(i);
      pianoKey.remove(i);
    }
  }

  //setting data previously received to current data reading
  preData = dataReceived;
}



/*
    data visualization option 3
 */
void draw_spiral() {
  float x = r * cos(theta);
  float y = r * sin(theta);
  noStroke();

  change_color();
  ellipse(x + canvasWidth/2, y + canvasHeight/2, 10, 10);

  theta = theta + 0.1;
  r= r + 0.45;
}



/*
    data visualization option 4
 */

void makeCircles(int depth) {
  if (depth == 0) {
    return;
  } else {

    strokeWeight(2);
    ellipse( x, y, depth*10, depth*10);
    makeCircles(depth-1);
  }
}



/*
    draws pause button
 */
void pauseButton()
{
  //button
  stroke(0);
  strokeWeight(1);
  fill(250, 240, 120); //yellow
  rect(30, 700, 100, 40, 8); //bottom left

  //text on button
  textSize(28);
  textAlign(CENTER, CENTER);
  fill(143, 102, 204); //purple
  text("PAUSE", 80, 716);
}



/*
    draws start button
 */
void startButton()
{
  //button
  stroke(0);
  strokeWeight(1);
  fill(143, 102, 204); //purple
  rect(30, 700, 100, 40, 8); //bottom left

  //text on button
  textSize(28);
  textAlign(CENTER, CENTER);
  fill(250, 240, 120); //yellow
  text("START", 80, 716);
}



/*
    draws switch button
 */
void switchButton()
{
  //button
  stroke(0);
  strokeWeight(1);
  if (dataViz == 1 || dataViz == 3)
  {
    fill(230, 90, 146); //pink
  } else {
    fill(200, 232, 180); //green
  }
  rect(160, 700, 120, 40, 8); //bottom left

  //text on button
  textSize(28);
  textAlign(CENTER, CENTER);
  if (dataViz == 1 || dataViz == 3)
  {
    fill(200, 232, 180); //green
  } else {
    fill(230, 90, 146); //pink
  }
  text("SWITCH", 220, 716);
}



/*
    when mouse is clicked, stuffs will happen depending on mouse's position
 */
void mouseClicked()
{
  if (mouseX >30 && mouseX <130 && mouseY > 700 && mouseY < 740)
  {
    //when pause button clicked, pause loop
    if (pause == false)
    {
      startButton();
      pause = true;
      noLoop();
    } else {
      pauseButton();
      pause = false;
      loop();
    }
  } else if (mouseX >160 && mouseX <280 && mouseY > 700 && mouseY < 740)
  {
    //switching animation
    if (dataViz == 1)
    {
      dataViz = 2;
    } else if (dataViz == 2) {
      dataViz = 3;
    } else if (dataViz == 3) {
      dataViz = 4;
    } else {
      dataViz = 1;
    }

    //resets everything
    background(255, 255, 255);
    xPos.clear();
    heightRect.clear();
    pianoKey.clear();
    count = 0;

    r = 0;
    theta = 0;

    //draws the pause and switch buttons on top of all the lines
    pauseButton();
    switchButton();
  } 
  //  else if (mouseX >30 && mouseX <130 && mouseY > 538 && mouseY < 578)
  //  {
  //    //debug, randomly changes value of data
  //    preData = 0;
  //    dataReceived = (int) random(1, 9);
  //  }
}



/*
    incrementing height if reading of preData = current data
 return: corresponding incremented height
 */
float height_incrementation()
{
  float val = minHeight + (incrementation * count);
  if (val > canvasHeight)
  {
    //if new max height range exceeds canvasHeight, just return canvasHeight 
    return canvasHeight;
  } else
  {
    return val;
  }
}



/*
    for debugging, randomly changes value of dataReceived when clicked
 */
void dataChange()
{
  //button
  stroke(0);
  strokeWeight(1);
  fill(64, 224, 208); //turquoise
  rect(30, 538, 100, 40, 10);

  //text on button
  textSize(20);
  textAlign(CENTER, CENTER);
  fill(0, 0, 0);
  text("diff color", 80, 553);
}


void change_color()
{
  //set drawing color depending on which key is touched
  if (dataReceived == 1)
  {
    fill(230, 90, 146); //pink
  } else if (dataReceived == 2)
  {
    fill(242, 56, 56); //red
  } else if (dataReceived == 3)
  {
    fill(237, 126, 71); //orange
  } else if (dataReceived == 4)
  {
    fill(250, 250, 105); //yellow
  } else if (dataReceived == 5)
  {
    fill(103, 152, 97); //green
  } else if (dataReceived == 6)
  {
    fill(64, 224, 208); //turquoise
  } else if (dataReceived == 7)
  {
    fill(55, 152, 237); //blue
  } else if (dataReceived == 8)
  {
    fill(141, 0, 207); //purple
  }
}

