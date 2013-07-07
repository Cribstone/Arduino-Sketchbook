/* 
   Created Dec 31, 2012
   by w9jds (Jeremy Shore)

   This uses the Cube class to create a cube in a process window. It then reads the
   serial information from COM3 (Arduino Esplora) and then runs it through the splitter
   function to get x, y, and z seperated. It then uses those to add/subtract from the
   x and y variable and then rotates the cube based on that variable.
*/

import processing.serial.*;

Serial myEsplora;
String SerInput;
int[] AccValues;

int x = 0;
int y = 0;
int z = 0;
Cube Box;

void setup() {
  size(700, 500, P3D);
  myEsplora = new Serial(this, "COM3", 9600);
  Box = new Cube(100, 100, 100);  
}

void draw() {
  if ( myEsplora.available() > 0) {
    SerInput = myEsplora.readStringUntil(';');
    AccValues = splitInput(SerInput);
    y += AccValues[0];
    x += AccValues[1];
    z = -1 * AccValues[2];
  }
  
  background(50);
  lights();
  translate(width/2, height/2, z);
  rotateX(radians(x));
  rotateY(radians(y));
  Box.create();
    
}

int[] splitInput(String SerInput) {

  if (SerInput == null){
    return AccValues;
  }
  
  int[] splitOutput = new int[3];
  int i = 0;
  int l = 0;
  String seg = "";
  
  while (SerInput.charAt(i) != ';') {
    if (SerInput.charAt(i) != ',') {
      seg += SerInput.charAt(i);
      i++;
    }
    else if (SerInput.charAt(i) == ',') {
      splitOutput[l] = int(seg);
      i++;
      seg = "";
      l++;
    }
  }
  splitOutput[l] = int(seg);
  return splitOutput;
}
