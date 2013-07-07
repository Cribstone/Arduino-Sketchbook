//Processing code:
import processing.serial.*;       

int xpos=90; // set x servo's value to mid point (0-180);
int ypos=90; // and the same here
Serial port; // The serial port we will be using

void setup()
{
  size(360, 360);
  frameRate(100);
  println(Serial.list()); // List COM-ports
  //select second com-port from the list (COM3 for my device)
  // You will want to change the [1] to select the correct device
  // Remember the list starts at [0] for the first option.
  port = new Serial(this, Serial.list()[0], 19200);
}

void draw()
{
  fill(175);
  rect(0,0,360,360);
  fill(255,0,0); //rgb value so RED
  rect(180, 175, mouseX-180, 10); //xpos, ypos, width, height
  fill(0,255,0); // and GREEN
  rect(175, 180, 10, mouseY-180);
  update(mouseX, mouseY);
}

void update(int x, int y)
{
  //Calculate servo postion from mouseX
  xpos= x/2;
  ypos = y/2;
  //Output the servo position ( from 0 to 180)
  port.write(xpos+"x");
  port.write(ypos+"y");
}

