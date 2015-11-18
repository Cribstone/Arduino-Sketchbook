import processing.serial.*;
//http://processing.org/examples/texturesphere.html

/**
 * Texture Sphere 
 * by Mike 'Flux' Chang (cleaned up by Aaron Koblin). 
 * Based on code by Toxi. 
 * 
 * A 3D textured sphere with simple rotation control.
 * Note: Controls will be inverted when sphere is upside down. 
 * Use an "arc ball" to deal with this appropriately.
 *
 * Edited to read data from the LSM303DLHC sensor to change the point of view.
 * Also placed the viewpoint to center of a sphere.
 *
 * By Eirik Haugen Murvold and Martin Holt Juliussen
 */ 

// The next line is needed if running in JavaScript Mode with Processing.js
/* @pjs preload="world32k.jpg"; */ 

PImage bg;
PImage texmap;

String starmap = "s3.jpg";

int sDetail = 35;  // Sphere detail setting
float rotationX = 0;
float rotationY = 0;
float velocityX = 0;
float velocityY = 0;
float globeRadius = 4000;
float pushBack = 0;

float[] cx, cz, sphereX, sphereY, sphereZ;
float sinLUT[];
float cosLUT[];
float SINCOS_PRECISION = 0.5;
int SINCOS_LENGTH = int(360.0 / SINCOS_PRECISION);

/*
 * Added variables.
 */
float accX, accY, accZ, magX, magY, magZ;
float xplacement, yplacement;
float xoff = 0.0f, yoff = 300.0f, zoff = 600.0f;
float Pi = 3.14159;
boolean yax = true;

boolean simulate = false;
boolean debug = false;

int serialInCount = 0;
String serialRead = "";
Serial serialPort;
PImage mask;
PGraphics maskmask;

/*
 * end added variables.
 */

void setup() {
  size(1280, 720, P3D);  
  serialPort = new Serial(this, Serial.list()[3], 115200);
  texmap = loadImage(starmap);    
  maskmask = createGraphics(width, height);
  maskmask.beginDraw();
  maskmask.background(255);
  maskmask.stroke(0);
  maskmask.fill(0);
  maskmask.ellipse(width/2, height/2, height, height);
  maskmask.endDraw();
  mask = createImage(width, height, RGB);
  mask.loadPixels();
  for (int i = 0; i < mask.pixels.length; i++) {
    mask.pixels[0] = color(0);
  }
  mask.updatePixels();
  mask.mask(maskmask);
  initializeSphere(sDetail);
}

void draw() {    
  background(0);            
  pushMatrix();
  translate(width * 0.5, height * 0.5, pushBack);
  if (yax) xplacement = radians(map(accX, -9.78f, 9.78f, 0, 360));
  if (!yax) yplacement = radians(-(atan2(magY, magX) * 180) / Pi);
  println(-(atan2(magY, magX) * 180) / Pi);
  
  rotateX(xplacement);
  rotateY(yplacement);
  renderGlobe();
  popMatrix();
  image(mask, 0, 0);
  if (simulate) {serialEvent(serialPort);}
}

void renderGlobe() {
  pushMatrix();
  //translate(width * 0.5, height * 0.5, pushBack);
  pushMatrix();
  noFill();
  stroke(255,200);
  strokeWeight(2);
  smooth();
  popMatrix();
  //lights();    
  pushMatrix();
  rotateX( radians(-rotationX) );  
  rotateY( radians(270 - rotationY) );
  fill(200);
  noStroke();
  textureMode(IMAGE);  
  texturedSphere(globeRadius, texmap);
  popMatrix();  
  popMatrix();
  rotationX += velocityX;
  rotationY += velocityY;
  velocityX *= 0.95;
  velocityY *= 0.95;
  
  // Implements mouse control (interaction will be inverse when sphere is  upside down)
  if(mousePressed){
    velocityX += (mouseY-pmouseY) * 0.01;
    velocityY -= (mouseX-pmouseX) * 0.01;
  }
}

void initializeSphere(int res)
{
  sinLUT = new float[SINCOS_LENGTH];
  cosLUT = new float[SINCOS_LENGTH];

  for (int i = 0; i < SINCOS_LENGTH; i++) {
    sinLUT[i] = (float) Math.sin(i * DEG_TO_RAD * SINCOS_PRECISION);
    cosLUT[i] = (float) Math.cos(i * DEG_TO_RAD * SINCOS_PRECISION);
  }

  float delta = (float)SINCOS_LENGTH/res;
  float[] cx = new float[res];
  float[] cz = new float[res];
  
  // Calc unit circle in XZ plane
  for (int i = 0; i < res; i++) {
    cx[i] = -cosLUT[(int) (i*delta) % SINCOS_LENGTH];
    cz[i] = sinLUT[(int) (i*delta) % SINCOS_LENGTH];
  }
  
  // Computing vertexlist vertexlist starts at south pole
  int vertCount = res * (res-1) + 2;
  int currVert = 0;
  
  // Re-init arrays to store vertices
  sphereX = new float[vertCount];
  sphereY = new float[vertCount];
  sphereZ = new float[vertCount];
  float angle_step = (SINCOS_LENGTH*0.5f)/res;
  float angle = angle_step;
  
  // Step along Y axis
  for (int i = 1; i < res; i++) {
    float curradius = sinLUT[(int) angle % SINCOS_LENGTH];
    float currY = -cosLUT[(int) angle % SINCOS_LENGTH];
    for (int j = 0; j < res; j++) {
      sphereX[currVert] = cx[j] * curradius;
      sphereY[currVert] = currY;
      sphereZ[currVert++] = cz[j] * curradius;
    }
    angle += angle_step;
  }
  sDetail = res;
}

// Generic routine to draw textured sphere
void texturedSphere(float r, PImage t) {
  int v1,v11,v2;
  r = (r + 240 ) * 0.33;
  beginShape(TRIANGLE_STRIP);
  texture(t);
  float iu=(float)(t.width-1)/(sDetail);
  float iv=(float)(t.height-1)/(sDetail);
  float u=0,v=iv;
  for (int i = 0; i < sDetail; i++) {
    vertex(0, -r, 0,u,0);
    vertex(sphereX[i]*r, sphereY[i]*r, sphereZ[i]*r, u, v);
    u+=iu;
  }
  vertex(0, -r, 0,u,0);
  vertex(sphereX[0]*r, sphereY[0]*r, sphereZ[0]*r, u, v);
  endShape();   
  
  // Middle rings
  int voff = 0;
  for(int i = 2; i < sDetail; i++) {
    v1=v11=voff;
    voff += sDetail;
    v2=voff;
    u=0;
    beginShape(TRIANGLE_STRIP);
    texture(t);
    for (int j = 0; j < sDetail; j++) {
      vertex(sphereX[v1]*r, sphereY[v1]*r, sphereZ[v1++]*r, u, v);
      vertex(sphereX[v2]*r, sphereY[v2]*r, sphereZ[v2++]*r, u, v+iv);
      u+=iu;
    }
  
    // Close each ring
    v1=v11;
    v2=voff;
    vertex(sphereX[v1]*r, sphereY[v1]*r, sphereZ[v1]*r, u, v);
    vertex(sphereX[v2]*r, sphereY[v2]*r, sphereZ[v2]*r, u, v+iv);
    endShape();
    v+=iv;
  }
  u=0;
  
  // Add the northern cap
  beginShape(TRIANGLE_STRIP);
  texture(t);
  for (int i = 0; i < sDetail; i++) {
    v2 = voff + i;
    vertex(sphereX[v2]*r, sphereY[v2]*r, sphereZ[v2]*r, u, v);
    vertex(0, r, 0,u,v+iv);    
    u+=iu;
  }
  vertex(sphereX[voff]*r, sphereY[voff]*r, sphereZ[voff]*r, u, v);
  endShape();
  
}


String serialSimulator() {
  //       accX accY  accZ magX   magY    magZ
  float accX, accY, accZ, magX, magY, magZ;
  accX = map(noise(xoff += 0.003f),0.0, 1.0, -10.0, 10.0);
  accY = map(noise(yoff += 0.003f),0.0, 1.0, -10.0, 10.0);
  accZ = map(noise(zoff += 0.003f),0.0, 1.0, -10.0, 10.0);
  return "" + accX + ":" + accY + ":" + accZ + ":0.36:-42.00:-123.27";
}



void serialEvent(Serial serialPort) {
  String serialIn = "";
  String [] serialxyz = new String [6];
  int serialAvail;
  if (!simulate) {serialAvail = serialPort.available();}
  else {serialAvail = 1;}
  if (serialAvail > 0) {
    if (!simulate) {serialIn = serialPort.readStringUntil(10);}
    if (simulate) {serialIn = serialSimulator();}
    if (serialIn != null) {
      try {
        serialxyz = serialIn.split(":");
        accX = Float.parseFloat(serialxyz[0]);
        accY = Float.parseFloat(serialxyz[1]);
        accZ = Float.parseFloat(serialxyz[2]);
        magX = Float.parseFloat(serialxyz[3]);
        magY = Float.parseFloat(serialxyz[4]);
        magZ = Float.parseFloat(serialxyz[5]);
        if (debug) {
          println("accX: " + accX);
          println("accY: " + accY);
          println("accZ: " + accZ);
          println("magX: " + magX);
          println("magY: " + magY);
          println("magZ: " + magZ);
        }

      } catch (Exception e) {
        println("I cought: " + e);
      }
    }
  }
}



void keyPressed() {
  switch (key) {
    case 'x': yax = yax ? false : true;
    break;
  }
}
