// Fun with Arduino by Rob Purser is licensed under a
// Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// http://creativecommons.org/licenses/by-nc-sa/3.0/deed.en_US
// Based on a work at https://sites.google.com/site/funarduino/
// Copyright Rob Purser, 2013

#include <Bridge.h>
#include <HttpClient.h>

int pinRed = 9;
int pinGreen = 6;
int pinBlue = 5;

// Assume that the serial connection is not available
bool serialEnabled = false;

void setup()
{
  pinMode(pinRed,OUTPUT);
  pinMode(pinGreen,OUTPUT);
  pinMode(pinBlue,OUTPUT);

  // Provide a indicator that the bridge connection is being set up
  setColor("red");
  Bridge.begin();
  Serial.begin(9600);

  // Provide indicator for serial connection set up
  setColor("yellow");

  // wait up to 5 seconds for the serial interface to start
  long waitStartedAt = millis();
  while(waitStartedAt + 5000 > millis() && !serialEnabled)
  {
    if(!Serial)
    {
      serialEnabled = false;
    }
    else
    {
      serialEnabled = true;
    }
  } 
  // Complete power on self test
  setColor("blue");
  delay(500);
  setColor("none");

}

void loop()
{
  HttpClient client;

  // Make a HTTP request:
  client.get("http://api.thingspeak.com/channels/1417/field/1/last.txt");

  // if there are incoming bytes available 
  // from the server, read them and print them:  
  unsigned int iColor = 0;
  char colorCommand[20];
  while (client.available())
  {
    char c = client.read();
    if(serialEnabled)
    {
      Serial.print(c);
    }
    colorCommand[iColor] = c;
    iColor++;
    if(iColor >= 19)  // remember that C is 0 based, so 19 is the 20th character in the array
    {
      // The text is more than just a color (like an error message) -- stop reading and flush.
      if(serialEnabled)
      {
        Serial.println();
        Serial.println("Got more text than expected -- abandon attempt");
      }
      delay(1000);
      return;
    }
    colorCommand[iColor] = 0;
  }
  if(serialEnabled)
  {
    Serial.println();
    Serial.flush();
    Serial.print("Color was: ");  
    Serial.println(colorCommand);
  }
  setColor(colorCommand);

  delay(5000);
}


String colorName[] = {
  "none","red","green","blue","cyan","white","warmwhite","purple","magenta","yellow","orange"};
int colorRGB[][3] = {     
  0,  0,  0, // "none"
  255,  0,  0, // "red"
  0,255,  0, // "green"
  0,  0,255, // "blue"
  0, 255,255, // "cyan",
  255, 50,100, // "white",
  255, 30, 10, // "warmwhite",
  128,  0, 25, // "purple",
  255,  0, 50, // "magenta",
  255, 100,  0, // "yellow",
  255, 10,  0}; // "orange"};


void setColor(String color)
{
  for(int iColor = 0; iColor <= 10; iColor++)
  {
    if(color == colorName[iColor])
    {
      if(serialEnabled)
      {
        Serial.print("Current Color is: "); 
        Serial.println(colorName[iColor]);
      }
      setRGB(colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]);
      return;
    }
  }
  if(serialEnabled)
  {
    Serial.print("Could not match color: "); 
    Serial.println(color);
  }
}

int previousRed = 0;
int previousGreen = 0;
int previousBlue = 0;

void setRGB(int redValue, int greenValue, int blueValue)
{
  if(serialEnabled)
  {
    Serial.print("Fading to: ");
    Serial.print(redValue);
    Serial.print(" , ");
    Serial.print(greenValue);
    Serial.print(" , ");
    Serial.println(blueValue);
  }
  for(float iLevel=0; iLevel<1; iLevel += 0.01)
  {
    analogWrite(pinRed,int(redValue * iLevel + previousRed * (1.0 - iLevel)));
    analogWrite(pinGreen,int(greenValue * iLevel + previousGreen * (1.0 - iLevel)));
    analogWrite(pinBlue,int(blueValue * iLevel + previousBlue * (1.0 - iLevel)));
    delay(20);
  }
  previousRed = redValue;
  previousGreen = greenValue;
  previousBlue = blueValue;
}



