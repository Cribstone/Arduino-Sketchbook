/*
This example shows the various abilities of the TouchScreenMenu library.
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

// create the array of items for the first sub menu
TouchScreenMenuItem subMenuItems[] = {
  TouchScreenMenuItem("<- Main Menu"),
  TouchScreenMenuItem("Left Justify"),
  TouchScreenMenuItem("Right Justify"),
  TouchScreenMenuItem("Small Font"),
  TouchScreenMenuItem("Spacing"),
  TouchScreenMenuItem("Padding"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the left justify sub menu
TouchScreenMenuItem leftSubMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Item 1"),
  TouchScreenMenuItem("Item 2"),
  TouchScreenMenuItem("Item 3"),
  TouchScreenMenuItem("Item 4"),
  TouchScreenMenuItem("Item 5"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the right justify sub menu
TouchScreenMenuItem rightSubMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Item 1"),
  TouchScreenMenuItem("Item 2"),
  TouchScreenMenuItem("Item 3"),
  TouchScreenMenuItem("Item 4"),
  TouchScreenMenuItem("Item 5"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the wide spacing sub menu
TouchScreenMenuItem spacingSubMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Item 1"),
  TouchScreenMenuItem("Item 2"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the wide padding sub menu
TouchScreenMenuItem paddingSubMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Item 1"),
  TouchScreenMenuItem("Item 2"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the small font sub menu
TouchScreenMenuItem smallSubMenuItems[] = {
  TouchScreenMenuItem("<- Back"),
  TouchScreenMenuItem("Item 1"),
  TouchScreenMenuItem("Item 2"),
  TouchScreenMenuItem("Item 3"),
  TouchScreenMenuItem("Item 4"),
  TouchScreenMenuItem("Item 5"),
  TouchScreenMenuItem("Item 6"),
  TouchScreenMenuItem("Item 7"),
  TouchScreenMenuItem("Item 8"),
  TouchScreenMenuItem("Item 9"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the array of items for the main menu
TouchScreenMenuItem mainMenuItems[] = {
  TouchScreenMenuItem("-> Sub Menus"),
  TouchScreenMenuItem("Gfx Functions"),
  TouchScreenMenuItem("Buttons"),
  TouchScreenMenuItem("Labels"),
  TouchScreenMenuItem("ENDOFMENU")
};

// create the various menus setting the items, font size, spacing, padding, justification and titles
TouchScreenMenu paddingSubMenu = TouchScreenMenu(paddingSubMenuItems, 2, 5, 20, CENTERJ, "Padding");
TouchScreenMenu spacingSubMenu = TouchScreenMenu(spacingSubMenuItems, 2, 40, 5, CENTERJ, "Spacing");
TouchScreenMenu smallSubMenu = TouchScreenMenu(smallSubMenuItems, 1, 10, 5, CENTERJ, "Small Font");
TouchScreenMenu rightSubMenu = TouchScreenMenu(rightSubMenuItems, 2, 10, 5, RIGHTJ, "Right Justify");
TouchScreenMenu leftSubMenu = TouchScreenMenu(leftSubMenuItems, 2, 10, 5, LEFTJ, "Left Justify");
TouchScreenMenu subMenu = TouchScreenMenu(subMenuItems, 2, 10, 5, CENTERJ, "Sub Menu");
TouchScreenMenu mainMenu = TouchScreenMenu(mainMenuItems, 2, 10, 10, CENTERJ, "Main Menu");

// keep track of which menu is the currently active one
TouchScreenMenu *curMenu = &mainMenu;

TouchScreenArea backFromGfx = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea backFromBtns = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);
TouchScreenArea backFromLbls = TouchScreenButton("<- Back", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 50, TSC.getScreenHeight() - 50, 2, 10);

TouchScreenArrowButton btn1 = TouchScreenArrowButton("btn1",TSC.createColor(0, 0, 0), TSC.createColor(255, 200, 0), 10, TSC.getScreenHeight() - 220, 30, 30, UP);
TouchScreenArrowButton btn2 = TouchScreenArrowButton("btn2",TSC.createColor(0, 0, 0), TSC.createColor(255, 0, 255), 50, TSC.getScreenHeight() - 220, 30, 50, DOWN);
TouchScreenArrowButton btn3 = TouchScreenArrowButton("btn3",TSC.createColor(0, 0, 0), TSC.createColor(0, 255, 255), 90, TSC.getScreenHeight() - 220, 30, 70, LEFT);
TouchScreenArrowButton btn4 = TouchScreenArrowButton("btn4",TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 130, TSC.getScreenHeight() - 220, 30, 90, RIGHT);
TouchScreenCircleButton btn5 = TouchScreenCircleButton("btn5",TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 10, 10, 70, 70);
TouchScreenCircleButton btn6 = TouchScreenCircleButton("btn6",TSC.createColor(0, 0, 0), TSC.createColor(0, 0, 255), 80, 10, 40, 40);
TouchScreenSlider btn7 = TouchScreenSlider("btn7",TSC.createColor(200, 200, 200), TSC.createColor(50, 50, 255), 5, TSC.getScreenHeight() - 110, 150, 40, HORIZONTAL);
TouchScreenSlider btn8 = TouchScreenSlider("btn8",TSC.createColor(255, 0, 0), TSC.createColor(200, 255, 0), TSC.getScreenWidth()-55, TSC.getScreenHeight() - 200, 40, 150, VERTICAL);

TouchScreenArea lbl1 = TouchScreenLabel("This is a label with bg.", TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 45, 10, 1, 2, true);
TouchScreenArea lbl2 = TouchScreenLabel("This is a label, no bg.", TSC.createColor(0, 0, 0), TSC.createColor(255, 255, 255), 45, 30, 1, 2, false);
TouchScreenArea lbl3 = TouchScreenLabel("Big label.", TSC.createColor(255, 255, 255), TSC.createColor(0, 0, 0), 0, 60, 2, 2, true);
TouchScreenArea lbl4 = TouchScreenLabel("Big label.", TSC.createColor(255, 0, 0), TSC.createColor(255, 255, 0), 0, 90, 2, 2, false);
TouchScreenArea lbl5 = TouchScreenLabel("Big label.", TSC.createColor(0, 200, 0), TSC.createColor(255, 0, 255), 0, 120, 3, 2, true);
TouchScreenArea lbl6 = TouchScreenLabel("Big label.", TSC.createColor(0, 200, 0), TSC.createColor(255, 0, 255), 0, 160, 3, 2, false);

void setup(void) {
  TSC.setBackColor(TSC.createColor(255, 255, 0)); // change the default background color
  TSC.init(); // make sure everything get initialized
  btn7.setValue(.5); // change the value on one of the sliders
  btn7.setPadding(5); // change the padding on one of the sliders
  btn8.setValue(.75); // change the value on one of the sliders
  btn8.setPadding(5); // change the padding on one of the sliders
  curMenu->draw(); // put up the main menu
}

void loop(void) {
  // handle the current menu
  if(curMenu!=NULL){
    // process the current menu
    TouchScreenMenuItem *item = curMenu->process(false);
    // check to see which, if any, menu item was pressed
    checkMenuSelection(item);
  }else{
    // if there isn't a current menu being displayed check all of the buttons
    // to see if any of them was pressed
    checkButtons();
  }
}

// check various buttons and perform actions if any was pressed
void checkButtons(){
  if(backFromGfx.process()){ // return from the graphics function screen
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
  }
  else if(backFromBtns.process()){ // return from the buttons screen
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
  }
  else if(backFromLbls.process()){ // return from the labels screen
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
  }
  else if(btn1.process()){
        //do something
  }
  else if(btn2.process()){
        //do something
  }
  else if(btn3.process()){
        //do something
  }
  else if(btn4.process()){
        //do something
  }
  else if(btn5.process()){
        //do something
  }
  else if(btn6.process()){
        //do something
  }
  else if(btn7.process()){
        //do something
  }
  else if(btn8.process()){
        //do something
  }
}

// check to see if any menu item was pressed and do something
void checkMenuSelection(TouchScreenMenuItem *item) {
  boolean handled = false;
  if(item != NULL){
    // main menu items 
    if(curMenu == &mainMenu){
      if(!strcmp(item->getText(),"-> Sub Menus")){
        curMenu = &subMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Gfx Functions")){
        curMenu = NULL;
        TSC.clearScreen();
        backFromGfx.draw();
        TSC.drawCircle(40, 20, 19, TSC.createColor(0,100,0), false);
        TSC.drawCircle(90, 20, 19, TSC.createColor(0,0,255), true);
        TSC.drawArc(40, 50, 19, 0, 180, TSC.createColor(0,0,0), false, false);
        TSC.drawArc(90, 50, 19, 90, 180, TSC.createColor(0,0,0), true, false);
        TSC.drawArc(140, 50, 19, 90, 225, TSC.createColor(0,0,0), true, true);
        TSC.drawRectangle(20, 80, 100, 20, TSC.createColor(255,0,0), false);
        TSC.drawRectangle(140, 80, 100, 20, TSC.createColor(255,0,0), true);
        TSC.drawLine(10, 110, 60, 110, TSC.createColor(0,0,255));
        TSC.drawLine(10, 110, 10, 140, TSC.createColor(0,0,255));
        TSC.drawLine(10, 110, 60, 140, TSC.createColor(0,0,255));
        handled = true;
      }
      else if(!strcmp(item->getText(),"Buttons")){
        curMenu = NULL;
        TSC.clearScreen();
        backFromBtns.draw();
        btn1.draw(); 
        btn2.draw();
        btn3.draw();
        btn4.draw();
        btn5.draw();
        btn6.draw();
        btn7.draw();
        btn8.draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Labels")){
        curMenu = NULL;
        TSC.clearScreen();
        backFromLbls.draw();
        lbl1.draw();
        lbl2.draw();
        lbl3.draw();
        lbl4.draw();
        lbl5.draw();
        lbl6.draw();
        handled = true;
      }
    }
    // sub menu items
    else if(curMenu == &subMenu){
      if(!strcmp(item->getText(),"<- Main Menu")){
        curMenu = &mainMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Left Justify")){
        curMenu = &leftSubMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Right Justify")){
        curMenu = &rightSubMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Small Font")){
        curMenu = &smallSubMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Spacing")){
        curMenu = &spacingSubMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
      else if(!strcmp(item->getText(),"Padding")){
        curMenu = &paddingSubMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // left justified menu items
    else if(curMenu == &leftSubMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &subMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // right justified menu items
    else if(curMenu == &rightSubMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &subMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // small font menu items
    else if(curMenu == &smallSubMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &subMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // spacing font menu items
    else if(curMenu == &spacingSubMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &subMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    // padding font menu items
    else if(curMenu == &paddingSubMenu){
      if(!strcmp(item->getText(),"<- Back")){
        curMenu = &subMenu;
        TSC.clearScreen();
        curMenu->draw();
        handled = true;
      }
    }
    
    // if the menu item didn't get handled redraw it unpressed
    if(handled==false)
        curMenu->drawItem(item,false);
  }
}

