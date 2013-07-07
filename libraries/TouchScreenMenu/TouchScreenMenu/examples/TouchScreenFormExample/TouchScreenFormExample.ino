/*
This example shows the various abilities of the TouchScreenMenu library.

In this example there are two different TouchScreenForm objects which contain
other objects.  Using TouchScreenForm you don't have to keep track of what
buttons or other controls should be handled.

See the readme.txt file for information on find the libraries this library uses.

 TouchScreenMenu Library. 
 
 2012 Copyright (c) Scott Shaver
 
 Authors: Scott Shaver
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <TouchScreenMenu.h>
#include <TouchScreen.h>
#include <TFT.h>
#include <cstddef.h>

TouchScreenForm form1 = TouchScreenForm("Form 1", 2);

TouchScreenForm form2 = TouchScreenForm("Form 2", 2);

TouchScreenLabel labels[] = {
  TouchScreenLabel("Big label.", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 0, 60, 2, 2, true),
  TouchScreenLabel("ENDOFFORM")
};

TouchScreenButton buttons[] = {
  TouchScreenButton("Next Form", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
  TouchScreenButton("ENDOFFORM")
};

TouchScreenCircleButton circleButtons[] ={
  TouchScreenCircleButton("Big",TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 10, 30, 70, 70),
  TouchScreenCircleButton("Small",TSC.createColor(0, 0, 0), TSC.createColor(0, 0, 255), 80, 30, 70, 40),
  TouchScreenCircleButton("ENDOFFORM")
};

TouchScreenArrowButton arrowButtons[] = {
  TouchScreenArrowButton("Up", TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 10, TSC.getScreenHeight() - 180 , 30, 30, UP),
  TouchScreenArrowButton("Down", TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 50, TSC.getScreenHeight() - 200, 30, 50, DOWN),
  TouchScreenArrowButton("Left", TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 255), 90, TSC.getScreenHeight() - 220, 30, 70, LEFT),
  TouchScreenArrowButton("Right", TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 130, TSC.getScreenHeight() - 240, 30, 90, RIGHT),
  TouchScreenArrowButton("ENDOFFORM")
};

TouchScreenSlider sliders[] = {
  TouchScreenSlider("Horiz", TSC.createColor(200, 200, 200), TSC.createColor(50, 50, 255), 5, TSC.getScreenHeight() - 110, 150, 40, HORIZONTAL),
  TouchScreenSlider("Vert", TSC.createColor(255, 0, 0), TSC.createColor(200, 255, 0), TSC.getScreenWidth()-55, TSC.getScreenHeight() - 200, 40, 150, VERTICAL),
  TouchScreenSlider("ENDOFFORM")
};

TouchScreenButton buttons2[] = {
  TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10),
  TouchScreenButton("ENDOFFORM")
};

// keep track of which form is the currently active one
TouchScreenForm *curForm = &form1;

void setup(void) {
  Serial.begin(57000);
  sliders[0].setValue(.5);
  sliders[1].setValue(.75);
  form1.setLabels(labels);
  form1.setButtons(buttons);
  form2.setArrowButtons(arrowButtons);
  form2.setCircleButtons(circleButtons);
  form2.setSliders(sliders);
  form2.setButtons(buttons2);

  TSC.setBackColor(TSC.createColor(255, 255, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized
  curForm->draw(); // put up the current form
}

void loop(void) {
  // handle the current form
  if(curForm!=NULL){
    // process the current menu
    TouchScreenArea *item = curForm->process(false);
    if(item!=NULL){
      if(curForm == &form1){
        // move to next form?
        if(!strcmp(item->getText(),"Next Form")){
          curForm = &form2;
          TSC.clearScreen();
          curForm->draw();
        }
      }
      else if(curForm == &form2){
        // move back to the first form?
        if(!strcmp(item->getText(),"<- Back")){
          curForm = &form1;
          TSC.clearScreen();
          curForm->draw();
        }
        else if(!strcmp(item->getText(),"Horiz")){
          Serial.println(((TouchScreenSlider *)item)->getValue(),DEC);
        }
        else if(!strcmp(item->getText(),"Vert")){
          Serial.println(((TouchScreenSlider *)item)->getValue(),DEC);
        }
        else if(!strcmp(item->getText(),"Up")){
          Serial.println("Up");
        }
        else if(!strcmp(item->getText(),"Down")){
          Serial.println("Down");
        }
        else if(!strcmp(item->getText(),"Left")){
          Serial.println("Left");
        }
        else if(!strcmp(item->getText(),"Right")){
          Serial.println("Right");
        }
        else if(!strcmp(item->getText(),"Big")){
          Serial.println("Big");
        }
        else if(!strcmp(item->getText(),"Small")){
          Serial.println("Small");
        }
      }
    }
  }
}

