import processing.serial.*;

Serial Port; // Create object from Serial class
public static final short portIndex = 1;

void setup()
{
  size(200, 200);
  // Open whatever port is the one you're using - See Chapter 4
 port = new Serial(this, Serial.list()[0], 9600); 
}

void draw()
{
  if ( port.available() > 0) {
    int y = Port.read();
    background(255);
    line(0,63-y,127,y);
   }
}
