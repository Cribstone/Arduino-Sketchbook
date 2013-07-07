/*
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
#include <Arduino.h>
#include <TouchScreen.h>
#include <TFT.h>

#ifndef _TouchScreenMenu_h
#define _TouchScreenMenu_h

// The dimensions of the font, in pixels, used by the TFT library
#define FONT_WIDTH 8
#define FONT_HEIGHT 8

//------------------------------------------------------------
// START: this stuff is used by the TFT library
//------------------------------------------------------------
#ifdef SEEEDUINO
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3 
#endif

#ifdef MEGA
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3 
#endif 

// These values define the best raw min/max values coming from the 
// touch screen, they get mapped from 0-SCREEN_WIDTH,0-SCREEN_HEIGHT
//Measured ADC values for (0,0) and (210-1,320-1)
//TS_MINX corresponds to ADC value when X = 0
//TS_MINY corresponds to ADC value when Y = 0
//TS_MAXX corresponds to ADC value when X = 240 -1
//TS_MAXY corresponds to ADC value when Y = 320 -1
// these are the values that worked with my touch screen
#define TS_MINX 152
#define TS_MAXX 874
#define TS_MINY 103
#define TS_MAXY 921

//------------------------------------------------------------
// END: this stuff is used by the TFT library
//------------------------------------------------------------

// your screen's width and height
#define SCREEN_WIDTH (MAX_X)
#define SCREEN_HEIGHT (MAX_Y)

// your screen's bits per color
// 18-bit color (262k), 5 bit of Red, 6 bits of Green, 5 bits of Blue 
// 24-bit color, 8 bit of Red, 8 bits of Green, 8 bits of Blue 
#define BLUE_BITS 5
#define GREEN_BITS 6
#define RED_BITS 5

class TouchScreenMenu;

// UI component justification values
typedef enum
{
    CENTERJ, LEFTJ, RIGHTJ
} Justify;

// UI component directional values
typedef enum
{
    RIGHT, LEFT, UP, DOWN
} Direction;

// UI component layout values
typedef enum
{
    HORIZONTAL, VERTICAL
} Layout;

//---------------------------------------------------------------------------------
// TouchScreenController
// The controller handles hiding the functionality of the TouchScreen and TFT 
// libraries. This class should accessed through the TSC global variable.
//---------------------------------------------------------------------------------
class TouchScreenController {
public:
    TouchScreenController();

    void init();
    TouchScreen *getTouchScreen();
    TFT *getTft();
    unsigned int getScreenWidth();
    unsigned int getScreenHeight();
    unsigned int getBackColor();
    void setBackColor(unsigned int backColor);
    void clearScreen();
    static unsigned int createColor(unsigned char r,unsigned char g,unsigned char b);
    void drawCircle(unsigned int xcenter,unsigned int ycenter,unsigned int radius, unsigned int foreColor, bool fill);
    void drawArc(unsigned int xcenter,unsigned int ycenter,unsigned int radius,unsigned int startAngle, unsigned int endAngle, unsigned int foreColor, bool asPiePiece, bool fill);
    double distance(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
    bool insideCircle(unsigned int px, unsigned int py,unsigned int cx, unsigned int cy,unsigned int radius);    
    void drawPixel(unsigned int poX, unsigned int poY,unsigned int color);
    void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);
    void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    void drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    void drawRectangle(unsigned int poX, unsigned int poY, unsigned int width,unsigned int height,unsigned int color, bool fill);
    void drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int color);
    void drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int color);
private:
    TouchScreen *_touchScreen;
    TFT *_tft;
    unsigned int _screenWidth;
    unsigned int _screenHeight;
    unsigned int _backColor;
};

// this is the global touch screen controller used by the classes
extern TouchScreenController TSC;

//---------------------------------------------------------------------------------
// TouchScreenArea
// Defines common functionality of items to be place on the screen.
//---------------------------------------------------------------------------------
class TouchScreenArea {
public:
    TouchScreenArea(char *text);
    TouchScreenArea(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int fontSize, unsigned int padding);
    
	virtual char *getTypeId(){return "TouchScreenArea";};
    virtual void getDims(unsigned int *x, unsigned int *y, unsigned int *w, unsigned int *h);
    virtual unsigned int getForeColor();
    virtual unsigned int getBackColor();
    virtual unsigned int getFontSize();
    virtual unsigned int getPadding();
    virtual void getPosition(unsigned int *x, unsigned int *y);
    virtual char *getText();
    virtual void setText(char *);
    virtual void setForeColor(unsigned int foreColor);
    virtual void setBackColor(unsigned int backColor);
    virtual void setFontSize(unsigned int fontSize);
    virtual void setPadding(unsigned int padding);
    virtual void setPosition(unsigned int x, unsigned int y);
    virtual void draw(bool pressed);
    virtual void draw();
    virtual bool checkForHit(unsigned int tx, unsigned int ty);
    virtual bool process(bool waitForClick);
    virtual bool process();
protected:
    virtual void calcDims();
    TouchScreenController *_controller;
    char *_text;
    unsigned int _foreColor;
    unsigned int _backColor;
    unsigned int _fontSize;
    unsigned int _padding;
    unsigned int _x;
    unsigned int _y;
    unsigned int _w;
    unsigned int _h;
};

//---------------------------------------------------------------------------------
// TouchScreenLabel
// A simple text label to place on the screen.
//---------------------------------------------------------------------------------
class TouchScreenLabel : public TouchScreenArea {
public:
    TouchScreenLabel(char *text);
    TouchScreenLabel(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int fontSize, unsigned int padding, bool drawBackground);

	virtual char *getTypeId(){return "TouchScreenLabel";};
	virtual void draw(bool pressed);
    virtual void draw();
protected:
    bool _drawBackground;
    virtual void calcDims();
};

//---------------------------------------------------------------------------------
// TouchScreenButton
// A base class for buttons that will be placed on the screen.
//---------------------------------------------------------------------------------
class TouchScreenButton : public TouchScreenArea {
public:
    TouchScreenButton(char *text);
    TouchScreenButton(char *text, unsigned int x, unsigned int y);
    TouchScreenButton(char *text, unsigned int x, unsigned int y, unsigned int fontSize, unsigned int padding);
    TouchScreenButton(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int fontSize, unsigned int padding);

	virtual char *getTypeId(){return "TouchScreenButton";};
	virtual void draw(bool pressed);
    virtual void draw();
protected:
    virtual void calcDims();
};

//---------------------------------------------------------------------------------
// TouchScreenGfxButton
// A base class for buttons that will use a custom draw() routine to display themselves.
//---------------------------------------------------------------------------------
class TouchScreenGfxButton : public TouchScreenArea {
public:
    TouchScreenGfxButton(char *text);
    TouchScreenGfxButton(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

	virtual char *getTypeId(){return "TouchScreenGfxButton";};
	virtual void draw(bool pressed) = 0;
    virtual void draw();
};

//---------------------------------------------------------------------------------
// TouchScreenCircleButton
// A button that is displayed as a circle.
//---------------------------------------------------------------------------------
class TouchScreenCircleButton : public TouchScreenGfxButton {
public:
    TouchScreenCircleButton(char *text);
    TouchScreenCircleButton(char *text,unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

	virtual char *getTypeId(){return "TouchScreenCircleButton";};
	virtual void draw(bool pressed);
    virtual void draw();
};

//---------------------------------------------------------------------------------
// TouchScreenSlider
// A button that displays a value (from 0 to 1) as a bar.  The value can be changed by pressing
// within the button
//---------------------------------------------------------------------------------
class TouchScreenSlider : public TouchScreenGfxButton {
public:
    TouchScreenSlider(char *text);
    TouchScreenSlider(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h, Layout layout);

	virtual char *getTypeId(){return "TouchScreenSlider";};
	virtual void draw(bool pressed);
    virtual void draw();
    virtual bool process();
    void setValue(float value);
    float getValue();
private:
    Layout _layout;
    float _value;
};

//---------------------------------------------------------------------------------
// TouchScreenArrowButton
// A button that is displayed as an arrow in one of four directions.
//---------------------------------------------------------------------------------
class TouchScreenArrowButton : public TouchScreenGfxButton {
public:
    TouchScreenArrowButton(char *text);
    TouchScreenArrowButton(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h, Direction dir);

	virtual char *getTypeId(){return "TouchScreenArrowButton";};
	virtual void draw(bool pressed);
    virtual void draw();
private:
    Direction _dir;
    void drawUp(bool pressed);
    void drawDown(bool pressed);
    void drawRight(bool pressed);
    void drawLeft(bool pressed);
};

//---------------------------------------------------------------------------------
// TouchScreenMenuItem
// An item that will be displyed for a menu.
//---------------------------------------------------------------------------------
class TouchScreenMenuItem {
public:
    TouchScreenMenuItem(char *text);
    TouchScreenMenuItem(char *text, unsigned int foreColor, unsigned int backColor);
    
    char *getText();
    void setText(char *);
    unsigned int getForeColor();
    unsigned int getBackColor();
    void setForeColor(unsigned int foreColor);
    void setBackColor(unsigned int backColor);
private:
    char *_text;
    unsigned int _foreColor;
    unsigned int _backColor;
};

//---------------------------------------------------------------------------------
// TouchScreenMenu
// A menu containing one or more TouchScreenMenuItems
//---------------------------------------------------------------------------------
class TouchScreenMenu {
public:
    TouchScreenMenu();
    TouchScreenMenu(TouchScreenMenuItem items[]);
    TouchScreenMenu(TouchScreenMenuItem items[], unsigned int fontSize, unsigned int spacing, unsigned int padding, Justify justify);
    TouchScreenMenu(TouchScreenMenuItem items[], unsigned int fontSize, unsigned int spacing, unsigned int padding, Justify justify, char *title);
    
    void setTitle(char *title);
    void setTitleColors(unsigned int foreColor, unsigned int backColor);
    void setItems(TouchScreenMenuItem items[]);
    void setFontSize(unsigned int fontSize);
    void setTitleFontSize(unsigned int fontSize);
    void setSpacing(unsigned int spacing);
    void setPadding(unsigned int padding);
    void setJustification(Justify justifyFlags);
    void draw();
    void getItemDims(unsigned int itemIndex, int *x, int *y, int *w, int *h);
    void getTitleDims(int *x, int *y, int *w, int *h);
    void drawItem(unsigned int itemIndex, bool pressed);
    void drawItem(TouchScreenMenuItem *item, bool pressed);
    TouchScreenMenuItem *checkForHit(unsigned int tx, unsigned int ty);
    TouchScreenMenuItem *process(bool waitForItem);
    void setClearScreenOnDraw(bool clearScreenOnDraw);
private:
    TouchScreenController *_controller;
    TouchScreenMenuItem *_items;
    char *_title;
    unsigned int _foreColor;
    unsigned int _backColor;
    unsigned int _titleFontSize;
    unsigned int _fontSize;
    unsigned int _spacing;
    unsigned int _padding;
    Justify _justify;
    bool _clearScreenOnDraw;
};


//---------------------------------------------------------------------------------
// TouchScreenForm
// Defines a single screen of controls that work together.
//---------------------------------------------------------------------------------
class TouchScreenForm {
public:
    TouchScreenForm();
    TouchScreenForm(char *title);
    TouchScreenForm(char *title, unsigned int titleFontSize);
    TouchScreenForm(char *title, unsigned int titleForeColor, unsigned int titleBackColor, unsigned int titleFontSize);
    
    void setTitle(char *title);
    void setTitleColors(unsigned int foreColor, unsigned int backColor);
    void setLabels(TouchScreenLabel widgets[]);
    void setButtons(TouchScreenButton widgets[]);
    void setCircleButtons(TouchScreenCircleButton widgets[]);
    void setArrowButtons(TouchScreenArrowButton widgets[]);
    void setSliders(TouchScreenSlider widgets[]);
    void setTitleFontSize(unsigned int fontSize);
    void setTitleSpacing(unsigned int titleSpacing);
    void setTitlePadding(unsigned int titlePadding);
    void draw();
    void getTitleDims(int *x, int *y, int *w, int *h);
    TouchScreenArea *checkForHit(unsigned int tx, unsigned int ty);
    TouchScreenArea *process(bool waitForItem);
    void setClearScreenOnDraw(bool clearScreenOnDraw);
private:
    TouchScreenController *_controller;
    TouchScreenLabel *_labels;
    TouchScreenButton *_buttons;
    TouchScreenCircleButton *_circleButtons;
    TouchScreenArrowButton *_arrowButtons;
    TouchScreenSlider *_sliders;
    char *_title;
    unsigned int _titleForeColor;
    unsigned int _titleBackColor;
    unsigned int _titleFontSize;
    unsigned int _titleSpacing;
    unsigned int _titlePadding;
    bool _clearScreenOnDraw;
};

#endif
