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

#include <TouchScreenMenu.h>
#include <string.h>
#include <cstddef.h>

//---------------------------------------------------------------------------------
// TouchScreenController
//---------------------------------------------------------------------------------
TouchScreenController::TouchScreenController() {
    _touchScreen = new TouchScreen(XP, YP, XM, YM, 300);
    _tft = &Tft;
    _screenWidth = SCREEN_WIDTH;
    _screenHeight = SCREEN_HEIGHT;
    setBackColor(0x0000);
}

void TouchScreenController::init(){
    _tft->init();  //init TFT library
    clearScreen();
}

unsigned int TouchScreenController::createColor(unsigned char r,unsigned char g,unsigned char b){
    return (map((unsigned int)r, 0, 255, 0, 31)<<(BLUE_BITS+GREEN_BITS)) | (map((unsigned int)g, 0, 255, 0, 63)<<BLUE_BITS) | map((unsigned int)b, 0, 255, 0, 31);
}

TouchScreen *TouchScreenController::getTouchScreen() {
    return _touchScreen;
}

TFT *TouchScreenController::getTft() {
    return _tft;
}

unsigned int TouchScreenController::getScreenWidth(){
    return _screenWidth;
}

unsigned int TouchScreenController::getScreenHeight(){
    return _screenHeight;
}

void TouchScreenController::clearScreen(){
    getTft()->fillRectangle(0, 0, getScreenWidth(), getScreenHeight(), getBackColor());
}    

unsigned int TouchScreenController::getBackColor(){
    return _backColor;
}

void TouchScreenController::setBackColor(unsigned int backColor){
    _backColor = backColor;
}

double TouchScreenController::distance(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
    return sqrt( pow(x2-x1,2) - pow(y2-y1,2) );
}

bool TouchScreenController::insideCircle(unsigned int px, unsigned int py,unsigned int cx, unsigned int cy,unsigned int radius){
    return distance(px,py,cx,cy)<=radius?true:false;
}

void TouchScreenController::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color){
    _tft->drawLine(x1, y1, x2, y2, color);
}

void TouchScreenController::drawPixel(unsigned int poX, unsigned int poY,unsigned int color){
    _tft->setPixel(poX,poY,color);
}

void TouchScreenController::drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color){
    _tft->drawVerticalLine(poX,poY,length,color);
}

void TouchScreenController::drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color){
    _tft->drawHorizontalLine(poX,poY,length,color);
}

void TouchScreenController::drawRectangle(unsigned int poX, unsigned int poY, unsigned int width,unsigned int height,unsigned int color, bool fill){
    if(fill==true)
        _tft->fillRectangle(poX, poY, width, height, color);
    else 
        _tft->drawRectangle(poX, poY, width, height, color);
}

void TouchScreenController::drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int color){
    _tft->drawChar(ascii,poX,poY,size,color);
}

void TouchScreenController::drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int color){
    if(string!=NULL)
        _tft->drawString(string,poX,poY,size,color);
}

void TouchScreenController::drawCircle(unsigned int cx,unsigned int cy,unsigned int radius, unsigned int foreColor, bool fill)
{ 
    unsigned int x=0;
    unsigned int y=radius;
    int  p=3-(2*radius);
    while(x<=y)
    { 
        if(fill==true)
        {
            drawLine(cx - x,cy + y,cx + x,cy + y,foreColor);
            drawLine(cx - x,cy - y,cx + x,cy - y,foreColor);
            drawLine(cx - y,cy + x,cx + y,cy + x,foreColor);
            drawLine(cx - y,cy - x,cx + y,cy - x,foreColor);
        }
        else
        {
            drawPixel(cx + x, cy + y, foreColor);
            drawPixel(cx - x, cy + y, foreColor);
            drawPixel(cx + x, cy - y, foreColor);
            drawPixel(cx - x, cy - y, foreColor);
            drawPixel(cx + y, cy + x, foreColor);
            drawPixel(cx - y, cy + x, foreColor);
            drawPixel(cx + y, cy - x, foreColor);
            drawPixel(cx - y, cy - x, foreColor);
        }
        
        if(p<0)
        {
            p+=(4*x)+6;
        }
        else
        {
            p+=4*(x-y)+10;
            y--;
        }
        x++;
    }
}

void TouchScreenController::drawArc(unsigned int cx,unsigned int cy,unsigned int radius,unsigned int startAngle, unsigned int endAngle, unsigned int foreColor, bool asPiePiece, bool fill)
{
    float rad = radius;
    float angInc = .1;
    float sa = startAngle * (3.14/180);
    float ea = endAngle * (3.14/180);
    unsigned int px = cx+(rad*cos(sa));
    unsigned int py = cy+(rad*sin(sa));
    float angle = sa + angInc;
    unsigned int px2;
    unsigned int py2;
    while(angle <= ea)
    {
        px2 = cx+(rad*cos(angle));
        py2 = cy+(rad*sin(angle));
        drawLine(px,py,px2,py2,foreColor);
        if(fill==true)
            drawLine(cx,cy,px,py,foreColor);
        angle += angInc;
        px = px2;
        py = py2;
    }
    angle = angle - angInc;
    if(angle < ea)
    {
        px2 = cx+(rad*cos(ea));
        py2 = cy+(rad*sin(ea));
        drawLine(px,py,px2,py2,foreColor);
        if(fill==true)
            drawLine(cx,cy,px2,py2,foreColor);
    }
    if(asPiePiece==true){
        px = cx+(rad*cos(sa));
        py = cy+(rad*sin(sa));
        drawLine(px,py,cx,cy,foreColor);
        drawLine(px2,py2,cx,cy,foreColor);
    }
}

// this is the global touch screen controller used by the classes
TouchScreenController TSC = TouchScreenController();

//---------------------------------------------------------------------------------
// TouchScreenArea
//---------------------------------------------------------------------------------

TouchScreenArea::TouchScreenArea(char *text){
    _controller = &TSC;
    setPadding(5);
    setText(text);
    setForeColor(0xffff);
    setBackColor(0x0000);
    setFontSize(2);
    setPosition(0,0);
    _w = 0;
    _h = 0;
}

TouchScreenArea::TouchScreenArea(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int fontSize, unsigned int padding){
    _controller = &TSC;
    setPadding(padding);
    setText(text);
    setForeColor(foreColor);
    setBackColor(backColor);
    setFontSize(fontSize);
    setPosition(x,y);
    _w = w;
    _h = h;
}

unsigned int TouchScreenArea::getForeColor(){
    return _foreColor;
}

unsigned int TouchScreenArea::getBackColor(){
    return _backColor;
}

void TouchScreenArea::getDims(unsigned int *x, unsigned int *y, unsigned int *w, unsigned int *h){
    *x = _x;
    *y = _y;
    *w = _w;
    *h = _h;
}

unsigned int TouchScreenArea::getFontSize(){
    return _fontSize;
}

unsigned int TouchScreenArea::getPadding(){
    return _padding;
}

void TouchScreenArea::getPosition(unsigned int *x, unsigned int *y){
    *x = _x;
    *y = _y;
}

void TouchScreenArea::setText(char *text){
    _text = text;
    calcDims();
}

char *TouchScreenArea::getText(){
    return _text;
}

void TouchScreenArea::setForeColor(unsigned int foreColor){
    _foreColor = foreColor;
}

void TouchScreenArea::setBackColor(unsigned int backColor){
    _backColor = backColor;
}

void TouchScreenArea::setFontSize(unsigned int fontSize){
    _fontSize = fontSize;
    calcDims();
}

void TouchScreenArea::setPadding(unsigned int padding){
    _padding = padding;
    calcDims();
}

void TouchScreenArea::setPosition(unsigned int x, unsigned int y){
    _x = x;
    _y = y;
}

void TouchScreenArea::draw(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, getBackColor(),false);
        _controller->drawString(getText(), x + _padding, y + _padding, _fontSize, getBackColor());
    }else{
        _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, getForeColor(),false);
        _controller->drawString(getText(), x + _padding, y + _padding, _fontSize, getForeColor());
    }
}

void TouchScreenArea::draw(){
    draw(false);
}

bool TouchScreenArea::checkForHit(unsigned int tx, unsigned int ty){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(tx>=x && tx<=x+w-1){
        if(ty>=y && ty<=y+h-1){
            draw(true);
            return true;
        }
    }
    return false;
}

bool TouchScreenArea::process(bool waitForClick){
    bool hit=false;
    bool wasHit = false;
    do{
        Point p = _controller->getTouchScreen()->getPoint();
        
        if (p.z > _controller->getTouchScreen()->pressureThreshhold) {
            p.x = map(p.x, TS_MINX, TS_MAXX, _controller->getScreenWidth(), 0);
            p.y = map(p.y, TS_MINY, TS_MAXY, _controller->getScreenHeight(), 0);
            hit = checkForHit(p.x , p.y);
            if(hit){
                draw(true);
            }
        }
    }while(hit==false && waitForClick);
    if(hit)
        draw(false);
    return hit;
}

bool TouchScreenArea::process(){
    return process(false);
}

void TouchScreenArea::calcDims(){
}

//---------------------------------------------------------------------------------
// TouchScreenLabel
//---------------------------------------------------------------------------------

TouchScreenLabel::TouchScreenLabel(char *text) : TouchScreenArea(text, 0xffff, 0x0000, 0, 0, 0, 0, 1, 5) {
    _drawBackground = false;
    calcDims();
}

TouchScreenLabel::TouchScreenLabel(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int fontSize, unsigned int padding, bool drawBackground) : TouchScreenArea(text, foreColor, backColor, x, y, 0, 0, fontSize, padding) {
    _drawBackground = drawBackground;
    calcDims();
}

void TouchScreenLabel::draw(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        if(_drawBackground)
            _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawString(getText(), x + _padding, y + _padding, _fontSize, getBackColor());
    }else{
        if(_drawBackground)
            _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawString(getText(), x + _padding, y + _padding, _fontSize, getForeColor());
    }
}

void TouchScreenLabel::draw(){
    draw(false);
}

void TouchScreenLabel::calcDims(){
    unsigned int fontPW = (FONT_WIDTH * _fontSize);
    unsigned int itemPH = (FONT_HEIGHT * _fontSize) + (_padding*2);
    unsigned int itemPW = getText()!=NULL?(strlen(getText()) * fontPW) + (_padding * 2):(_padding * 2);
    
    _w = itemPW;
    _h = itemPH;
}

//---------------------------------------------------------------------------------
// TouchScreenButton
//---------------------------------------------------------------------------------

TouchScreenButton::TouchScreenButton(char *text) : TouchScreenArea(text, 0xffff, 0x0000, 0, 0, 0, 0, 1, 2) {
    calcDims();
}

TouchScreenButton::TouchScreenButton(char *text, unsigned int x, unsigned int y, unsigned int fontSize, unsigned int padding) : TouchScreenArea(text, 0xffff, 0x0000, x, y, 0, 0, fontSize, padding) {
    calcDims();
}

TouchScreenButton::TouchScreenButton(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int fontSize, unsigned int padding) : TouchScreenArea(text, foreColor, backColor, x, y, 0, 0, fontSize, padding) {
    calcDims();
}

void TouchScreenButton::calcDims(){
    unsigned int fontPW = (FONT_WIDTH * _fontSize);
    unsigned int itemPH = (FONT_HEIGHT * _fontSize) + (_padding*2);
    unsigned int itemPW = getText()!=NULL?(strlen(getText()) * fontPW) + (_padding * 2):(_padding * 2);
    
    _w = itemPW;
    _h = itemPH;
}

void TouchScreenButton::draw(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, getBackColor(),false);
        _controller->drawString(getText(), x + _padding, y + _padding, _fontSize, getBackColor());
    }else{
        _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, getForeColor(),false);
        _controller->drawString(getText(), x + _padding, y + _padding, _fontSize, getForeColor());
    }
}

void TouchScreenButton::draw(){
    draw(false);
}

//---------------------------------------------------------------------------------
// TouchScreenGfxButton
//---------------------------------------------------------------------------------

TouchScreenGfxButton::TouchScreenGfxButton(char *text) : TouchScreenArea(text) {
}

TouchScreenGfxButton::TouchScreenGfxButton(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h) : TouchScreenArea(text, foreColor, backColor, x, y, w, h, 1, 2) {
}

void TouchScreenGfxButton::draw(){
    draw(false);
}

//---------------------------------------------------------------------------------
// TouchScreenCircleButton
//---------------------------------------------------------------------------------

TouchScreenCircleButton::TouchScreenCircleButton(char *text) : TouchScreenGfxButton(text) {
}

TouchScreenCircleButton::TouchScreenCircleButton(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h) : TouchScreenGfxButton(text, foreColor, backColor, x, y, w, h) {
}

void TouchScreenCircleButton::draw(bool pressed){
    unsigned int cx = _x+(_w/2);
    unsigned int cy = _y+(_h/2);
    unsigned int radius = _w<=_h?(_w/2):(_h/2);
    
    if(pressed){
        _controller->drawCircle(cx, cy, radius, getForeColor(), true);
        _controller->drawCircle(cx, cy, radius, getBackColor(), false);
    }else{
        _controller->drawCircle(cx, cy, radius, getBackColor(), true);
        _controller->drawCircle(cx, cy, radius, getForeColor(), false);
    }
}

void TouchScreenCircleButton::draw(){
    draw(false);
}

//---------------------------------------------------------------------------------
// TouchScreenSlider
//---------------------------------------------------------------------------------

TouchScreenSlider::TouchScreenSlider(char *text) : TouchScreenGfxButton(text) {
}

TouchScreenSlider::TouchScreenSlider(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h, Layout layout) : TouchScreenGfxButton(text,foreColor, backColor, x, y, w, h) {
    _layout = layout;
	setPadding(5);
}

void TouchScreenSlider::draw(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, getBackColor(),false);
        if(_layout==HORIZONTAL){
            unsigned int thumbSize = map((int)(_value*100), 0, 100, 1, w - (_padding*2));
            _controller->drawRectangle(x + _padding, y + _padding, thumbSize, h - (_padding*2), getBackColor(),true);
            for(int lx=x+_padding+10;lx<w - (_padding*2) + x;lx+=10){ 
                _controller->drawVerticalLine(lx, y + _padding, h - (_padding*2), getForeColor());
            }
        }else if(_layout==VERTICAL){
            unsigned int thumbSize = map((int)(_value*100), 0, 100, 1, h - (_padding*2));
            _controller->drawRectangle(x + _padding, y + _padding + ((h - (_padding*2))-thumbSize), w - (_padding*2), thumbSize,  getBackColor(),true);
            for(int ly=y+_padding+10;ly<h - (_padding*2) + y;ly+=10){ 
                _controller->drawHorizontalLine(x + _padding, ly, w - (_padding*2), getForeColor());
            }
        }
    }else{
        _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, getForeColor(),false);
        if(_layout==HORIZONTAL){
            unsigned int thumbSize = map((int)(_value*100), 0, 100, 1, w - (_padding*2));
            _controller->drawRectangle(x + _padding, y + _padding, thumbSize, h - (_padding*2), getForeColor(),true);
            for(int lx=x+_padding+10;lx<w - (_padding*2) + x;lx+=10){ 
                _controller->drawVerticalLine(lx, y + _padding, h - (_padding*2), getBackColor());
            }
        }else if(_layout==VERTICAL){
            unsigned int thumbSize = map((int)(_value*100), 0, 100, 1, h - (_padding*2));
            _controller->drawRectangle(x + _padding, y + _padding + ((h - (_padding*2))-thumbSize), w - (_padding*2), thumbSize,  getForeColor(),true);
            for(int ly=y+_padding+10;ly<h - (_padding*2) + y;ly+=10){ 
                _controller->drawHorizontalLine(x + _padding, ly, w - (_padding*2), getBackColor());
            }
        }
    }
}

void TouchScreenSlider::draw(){
    draw(false);
}

bool TouchScreenSlider::process(){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    bool hit=false;
    bool wasChanged = false;
    //do{
        Point p = _controller->getTouchScreen()->getPoint();
        
        if (p.z > _controller->getTouchScreen()->pressureThreshhold) {
            p.x = map(p.x, TS_MINX, TS_MAXX, _controller->getScreenWidth(), 0);
            p.y = map(p.y, TS_MINY, TS_MAXY, _controller->getScreenHeight(), 0);
            hit = checkForHit(p.x , p.y);
            if(hit){
                wasChanged=true;
                if(_layout==HORIZONTAL){
                    unsigned int value = map(p.x, x, x+w-1, 0, 100);
                    setValue((float)value/100);
                    draw(true);
                }else if(_layout==VERTICAL){
                    unsigned int value = map(p.y, y+h-1, y, 0, 100);
                    setValue((float)value/100);
                    draw(true);
                }
            }
        }
    //}while(hit==true);
    return wasChanged;
}

void TouchScreenSlider::setValue(float value){
    _value = value;
}

float TouchScreenSlider::getValue(){
    return _value;
}

//---------------------------------------------------------------------------------
// TouchScreenArrowButton
//---------------------------------------------------------------------------------

TouchScreenArrowButton::TouchScreenArrowButton(char *text) : TouchScreenGfxButton(text) {
}

TouchScreenArrowButton::TouchScreenArrowButton(char *text, unsigned int foreColor, unsigned int backColor, unsigned int x, unsigned int y, unsigned int w, unsigned int h, Direction dir) : TouchScreenGfxButton(text, foreColor, backColor, x, y, w, h) {
    _dir = dir;
}

void TouchScreenArrowButton::draw(bool pressed){
    switch(_dir){
        case UP:
            drawUp(pressed);
            break;
        case DOWN:
            drawDown(pressed);
            break;
        case LEFT:
            drawLeft(pressed);
            break;
        case RIGHT:
            drawRight(pressed);
            break;
    }
}

void TouchScreenArrowButton::draw(){
    draw(false);
}

void TouchScreenArrowButton::drawUp(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, getBackColor(),false);
        _controller->drawLine(x + (w/2), y + _padding, x + _padding, y + h - _padding, getBackColor()); //left side
        _controller->drawLine(x + (w/2), y + _padding, x + w - 1 - _padding, y + h - _padding, getBackColor()); //right side
        _controller->drawLine(x + _padding, y + h - _padding, x + w - 1 - _padding, y + h - _padding, getBackColor()); //bottom side
    }else{
        _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, getForeColor(),false);
        _controller->drawLine(x + (w/2), y + _padding, x + _padding, y + h - _padding, getForeColor()); //left side
        _controller->drawLine(x + (w/2), y + _padding, x + w - 1 - _padding, y + h - _padding, getForeColor()); //right side
        _controller->drawLine(x + _padding, y + h - _padding, x + w - 1 - _padding, y + h - _padding, getForeColor()); //bottom side
    }
}

void TouchScreenArrowButton::drawDown(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, getBackColor(),false);
        _controller->drawLine(x + (w/2), y + h - _padding, x + _padding, y + _padding, getBackColor()); //left side
        _controller->drawLine(x + (w/2), y + h - _padding, x + w - 1 - _padding, y + _padding, getBackColor()); //right side
        _controller->drawLine(x + _padding, y + _padding, x + w - 1 - _padding, y + _padding, getBackColor()); //bottom side
    }else{
        _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, getForeColor(),false);
        _controller->drawLine(x + (w/2), y + h - _padding, x + _padding, y + _padding, getForeColor()); //left side
        _controller->drawLine(x + (w/2), y + h - _padding, x + w - 1 - _padding, y + _padding, getForeColor()); //right side
        _controller->drawLine(x + _padding, y + _padding, x + w - 1 - _padding, y + _padding, getForeColor()); //bottom side
    }
}

void TouchScreenArrowButton::drawLeft(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, getBackColor(),false);
        _controller->drawLine(x + _padding, y + (h/2), x + w -1 - _padding, y + _padding, getBackColor()); //top side
        _controller->drawLine(x + _padding, y + (h/2), x + w - 1 - _padding, y + h - 1 - _padding, getBackColor()); //bottom side
        _controller->drawLine(x + w -1 - _padding, y + _padding, x + w - 1 - _padding, y + h - 1 - _padding, getBackColor()); //right side
    }else{
        _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, getForeColor(),false);
        _controller->drawLine(x + _padding, y + (h/2), x + w -1 - _padding, y + _padding, getForeColor()); //top side
        _controller->drawLine(x + _padding, y + (h/2), x + w - 1 - _padding, y + h - 1 - _padding, getForeColor()); //bottom side
        _controller->drawLine(x + w -1 - _padding, y + _padding, x + w - 1 - _padding, y + h - 1 - _padding, getForeColor()); //right side
    }
}

void TouchScreenArrowButton::drawRight(bool pressed){
    unsigned int x, y, w, h;
    getDims(&x, &y, &w, &h);
    
    if(pressed){
        _controller->drawRectangle(x, y, w, h, getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, getBackColor(),false);
        _controller->drawLine(x + _padding, y + _padding, x + w -1 - _padding, y + (h/2), getBackColor()); //top side
        _controller->drawLine(x + _padding, y + h - 1 - _padding, x + w - 1 - _padding, y + (h/2), getBackColor()); //bottom side
        _controller->drawLine(x + _padding, y + _padding, x + _padding, y + h - 1 - _padding, getBackColor()); //left side
    }else{
        _controller->drawRectangle(x, y, w, h, getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, getForeColor(),false);
        _controller->drawLine(x + _padding, y + _padding, x + w -1 - _padding, y + (h/2), getForeColor()); //top side
        _controller->drawLine(x + _padding, y + h - 1 - _padding, x + w - 1 - _padding, y + (h/2), getForeColor()); //bottom side
        _controller->drawLine(x + _padding, y + _padding, x + _padding, y + h - 1 - _padding, getForeColor()); //left side
    }
}

//---------------------------------------------------------------------------------
// TouchScreenMenuItem
//---------------------------------------------------------------------------------

TouchScreenMenuItem::TouchScreenMenuItem(char *text){
    setText(text);
    setForeColor(0xffff);
    setBackColor(0xf000);
}

TouchScreenMenuItem::TouchScreenMenuItem(char *text, unsigned int foreColor, unsigned int backColor){
    setText(text);
    setForeColor(foreColor);
    setBackColor(backColor);
}

unsigned int TouchScreenMenuItem::getForeColor(){
    return _foreColor;
}

unsigned int TouchScreenMenuItem::getBackColor(){
    return _backColor;
}

void TouchScreenMenuItem::setForeColor(unsigned int foreColor){
    _foreColor = foreColor;
}

void TouchScreenMenuItem::setBackColor(unsigned int backColor){
    _backColor = backColor;
}

char *TouchScreenMenuItem::getText(){
    return _text;
}

void TouchScreenMenuItem::setText(char *text){
    _text = text;
}

//---------------------------------------------------------------------------------
// TouchScreenMenu
//---------------------------------------------------------------------------------

TouchScreenMenu::TouchScreenMenu(){
    _controller = &TSC;
    setSpacing(5);
    setPadding(2);
    setJustification(LEFTJ);
    setFontSize(2);
    setTitleFontSize(2);
    setItems(NULL);
    setTitle(NULL);
    setTitleColors(0xffff, 0x001f);
    setClearScreenOnDraw(false);
}

TouchScreenMenu::TouchScreenMenu(TouchScreenMenuItem items[]){
    _controller = &TSC;
    setSpacing(5);
    setPadding(2);
    setFontSize(2);
    setTitleFontSize(2);
    setJustification(LEFTJ);
    setItems(items);
    setTitle(NULL);
    setTitleColors(0xffff, 0x001f);
    setClearScreenOnDraw(false);
}

TouchScreenMenu::TouchScreenMenu(TouchScreenMenuItem items[], unsigned int fontSize, unsigned int spacing, unsigned int padding, Justify justify){
    _controller = &TSC;
    setSpacing(spacing);
    setPadding(padding);
    setFontSize(fontSize);
    setTitleFontSize(2);
    setJustification(justify);
    setItems(items);
    setTitle(NULL);
    setTitleColors(0xffff, 0x001f);
    setClearScreenOnDraw(false);
}

TouchScreenMenu::TouchScreenMenu(TouchScreenMenuItem items[], unsigned int fontSize, unsigned int spacing, unsigned int padding, Justify justify, char *title){
    _controller = &TSC;
    setSpacing(spacing);
    setPadding(padding);
    setFontSize(fontSize);
    setTitleFontSize(2);
    setJustification(justify);
    setItems(items);
    setTitle(title);
    setTitleColors(0xffff, 0x001f);
    setClearScreenOnDraw(false);
}

void TouchScreenMenu::setTitle(char *title){
    _title = title;
}

void TouchScreenMenu::setTitleColors(unsigned int foreColor, unsigned int backColor){
    _foreColor = foreColor;
    _backColor = backColor;
}

void TouchScreenMenu::setItems(TouchScreenMenuItem items[]){
    _items = items;
}

void TouchScreenMenu::setFontSize(unsigned int fontSize){
    _fontSize = fontSize;
}

void TouchScreenMenu::setTitleFontSize(unsigned int fontSize){
    _titleFontSize = fontSize;
}

void TouchScreenMenu::setSpacing(unsigned int spacing){
    _spacing = spacing;
}

void TouchScreenMenu::setPadding(unsigned int padding){
    _padding = padding;
}

void TouchScreenMenu::setJustification(Justify justify){
    _justify = justify;
}

void TouchScreenMenu::setClearScreenOnDraw(bool clearScreenOnDraw){
    _clearScreenOnDraw = clearScreenOnDraw;
}

void TouchScreenMenu::draw(){
    if(_clearScreenOnDraw)
        _controller->clearScreen();

    int x, y, w, h;
    getTitleDims(&x, &y, &w, &h);
    _controller->drawRectangle(0, 0, _controller->getScreenWidth()-1, h, _backColor,true);
    _controller->drawRectangle(0, 0, _controller->getScreenWidth()-1, h, _foreColor,false);
    _controller->drawString(_title, x + _padding, y + _padding, _titleFontSize, _foreColor);
    
    int length = 0;
    while(1){
        if(strcmp(_items[length].getText(),"ENDOFMENU")==0)
            break;
        length++;
    }
    
    for (int i = 0; i < length; i++){
        drawItem(i, false);
    }
}

void TouchScreenMenu::drawItem(unsigned int itemIndex, bool pressed){
    int x, y, w, h;
    getItemDims(itemIndex, &x, &y, &w, &h);
        
    if(pressed){
        _controller->drawRectangle(x, y, w, h, _items[itemIndex].getForeColor(),true);
        _controller->drawRectangle(x, y, w, h, _items[itemIndex].getBackColor(),false);
        _controller->drawString(_items[itemIndex].getText(), x + _padding, y + _padding, _fontSize, _items[itemIndex].getBackColor());
    }else{
        _controller->drawRectangle(x, y, w, h, _items[itemIndex].getBackColor(),true);
        _controller->drawRectangle(x, y, w, h, _items[itemIndex].getForeColor(),false);
        _controller->drawString(_items[itemIndex].getText(), x + _padding, y + _padding, _fontSize, _items[itemIndex].getForeColor());
    }
}

void TouchScreenMenu::drawItem(TouchScreenMenuItem *item, bool pressed){
    if(item==NULL) return;
    int index = 0;
    while(1){
        if(strcmp(_items[index].getText(),item->getText())==0){
            drawItem(index,pressed);
            return;
        }
        index++;
    }
}

TouchScreenMenuItem *TouchScreenMenu::checkForHit(unsigned int tx, unsigned int ty){
    int length = 0;
    while(1){
        if(strcmp(_items[length].getText(),"ENDOFMENU")==0)
            break;
        length++;
    }
    
    for (int i = 0; i < length; i++){
        int x, y, w, h;
        getItemDims(i, &x, &y, &w, &h);
        if(tx>=x && tx<=x+w-1){
            if(ty>=y && ty<=y+h-1){
                drawItem(i, true);
                return &(_items[i]);
            }
        }
    }
    
    return NULL;
}

void TouchScreenMenu::getItemDims(unsigned int itemIndex, int *x, int *y, int *w, int *h){
    int tx, ty, tw, th;
    getTitleDims(&tx, &ty, &tw, &th);

    unsigned int topY = ((FONT_HEIGHT * _fontSize)+(_padding*2)+_spacing);
    unsigned int fontPW = (FONT_WIDTH * _fontSize);
    unsigned int itemPH = (FONT_HEIGHT * _fontSize) + (_padding*2);
    unsigned int itemPW = (strlen(_items[itemIndex].getText()) * fontPW) + (_padding * 2);
        
    int leftOffset = 0;
    if(_justify == RIGHTJ)
        leftOffset = _controller->getScreenWidth() - itemPW - 1;
    else if(_justify == CENTERJ)
        leftOffset = (_controller->getScreenWidth()/2) - (itemPW/2);
    else if(_justify == LEFTJ)
        leftOffset = 0;
        
    *x = leftOffset;
    *y = (itemIndex * topY) + (ty+th+_spacing);
    *w = itemPW;
    *h = itemPH;
}

void TouchScreenMenu::getTitleDims(int *x, int *y, int *w, int *h){
    if(_title==NULL){
        *x = 0;
        *y = 0;
        *w = 0;
        *h = 0;
        return;
    }
    unsigned int fontPW = (FONT_WIDTH * _titleFontSize);
    unsigned int itemPH = (FONT_HEIGHT * _titleFontSize) + (_padding*2);
    unsigned int itemPW = (strlen(_title) * fontPW) + (_padding * 2);
    
    *x = (_controller->getScreenWidth()/2) - (itemPW/2);
    *y = 0;
    *w = itemPW;
    *h = itemPH;
}

TouchScreenMenuItem *TouchScreenMenu::process(bool waitForItem){
    TouchScreenMenuItem *item = 0;
    do{
        Point p = _controller->getTouchScreen()->getPoint();
    
        if (p.z > _controller->getTouchScreen()->pressureThreshhold) {
            p.x = map(p.x, TS_MINX, TS_MAXX, _controller->getScreenWidth(), 0);
            p.y = map(p.y, TS_MINY, TS_MAXY, _controller->getScreenHeight(), 0);
            item = checkForHit(p.x , p.y);
        }
    }while(item==NULL && waitForItem);
    
    return item;
}

//---------------------------------------------------------------------------------
// TouchScreenForm
//---------------------------------------------------------------------------------

TouchScreenForm::TouchScreenForm(){
    _controller = &TSC;
    setTitleSpacing(2);
    setTitlePadding(5);
    setTitleFontSize(2);
    setTitle(NULL);
    setTitleColors(0xffff, 0x001f);
    setClearScreenOnDraw(false);
    setLabels(NULL);
    setButtons(NULL);
    setCircleButtons(NULL);
    setArrowButtons(NULL);
    setSliders(NULL);
}

TouchScreenForm::TouchScreenForm(char *title){
    _controller = &TSC;
    setTitleSpacing(2);
    setTitlePadding(5);
    setTitleFontSize(2);
    setTitle(title);
    setTitleColors(0xffff, 0x001f);
    setClearScreenOnDraw(false);
    setLabels(NULL);
    setButtons(NULL);
    setCircleButtons(NULL);
    setArrowButtons(NULL);
    setSliders(NULL);
}

TouchScreenForm::TouchScreenForm(char *title, unsigned int titleFontSize){
    _controller = &TSC;
    setTitleSpacing(2);
    setTitlePadding(5);
    setTitleFontSize(titleFontSize);
    setTitle(title);
    setTitleColors(0xffff, 0x001f);
    setClearScreenOnDraw(false);
    setLabels(NULL);
    setButtons(NULL);
    setCircleButtons(NULL);
    setArrowButtons(NULL);
    setSliders(NULL);
}

TouchScreenForm::TouchScreenForm(char *title, unsigned int titleForeColor, unsigned int titleBackColor, unsigned int titleFontSize){
    _controller = &TSC;
    setTitleSpacing(2);
    setTitlePadding(5);
    setTitleFontSize(titleFontSize);
    setTitle(title);
    setTitleColors(titleForeColor, titleBackColor);
    setClearScreenOnDraw(false);
    setLabels(NULL);
    setButtons(NULL);
    setCircleButtons(NULL);
    setArrowButtons(NULL);
    setSliders(NULL);
}

void TouchScreenForm::setTitle(char *title){
    _title = title;
}

void TouchScreenForm::setTitleColors(unsigned int titleForeColor, unsigned int titleBackColor){
    _titleForeColor = titleForeColor;
    _titleBackColor = titleBackColor;
}

void TouchScreenForm::setTitleFontSize(unsigned int fontSize){
    _titleFontSize = fontSize;
}

void TouchScreenForm::setClearScreenOnDraw(bool clearScreenOnDraw){
    _clearScreenOnDraw = clearScreenOnDraw;
}

void TouchScreenForm::setTitleSpacing(unsigned int titleSpacing){
    _titleSpacing = titleSpacing;
}

void TouchScreenForm::setTitlePadding(unsigned int titlePadding){
    _titlePadding = titlePadding;
}

void TouchScreenForm::getTitleDims(int *x, int *y, int *w, int *h){
    if(_title==NULL){
        *x = 0;
        *y = 0;
        *w = 0;
        *h = 0;
        return;
    }
    unsigned int fontPW = (FONT_WIDTH * _titleFontSize);
    unsigned int itemPH = (FONT_HEIGHT * _titleFontSize) + (_titlePadding*2);
    unsigned int itemPW = (strlen(_title) * fontPW) + (_titlePadding * 2);
    
    *x = (_controller->getScreenWidth()/2) - (itemPW/2);
    *y = 0;
    *w = itemPW;
    *h = itemPH;
}

void TouchScreenForm::draw(){
    if(_clearScreenOnDraw)
        _controller->clearScreen();

    int x, y, w, h;
    getTitleDims(&x, &y, &w, &h);
    _controller->drawRectangle(0, 0, _controller->getScreenWidth()-1, h, _titleBackColor,true);
    _controller->drawRectangle(0, 0, _controller->getScreenWidth()-1, h, _titleForeColor,false);
    _controller->drawString(_title, x + _titlePadding, y + _titlePadding, _titleFontSize, _titleForeColor);
    
    int length = 0;
    while(1 && _labels!=NULL){
        if(strcmp(_labels[length].getText(),"ENDOFFORM")==0)
            break;
		_labels[length].draw();
        length++;
    }

    length = 0;
    while(1 && _buttons!=NULL){
        if(strcmp(_buttons[length].getText(),"ENDOFFORM")==0)
            break;
		_buttons[length].draw();
        length++;
    }

    length = 0;
    while(1 && _circleButtons!=NULL){
        if(strcmp(_circleButtons[length].getText(),"ENDOFFORM")==0)
            break;
		_circleButtons[length].draw();
        length++;
    }

    length = 0;
    while(1 && _arrowButtons!=NULL){
        if(strcmp(_arrowButtons[length].getText(),"ENDOFFORM")==0)
            break;
		_arrowButtons[length].draw();
        length++;
    }

    length = 0;
    while(1 && _sliders!=NULL){
        if(strcmp(_sliders[length].getText(),"ENDOFFORM")==0)
            break;
		_sliders[length].draw();
        length++;
    }
}

TouchScreenArea *TouchScreenForm::process(bool waitForItem){
    do{
        /*Point p = _controller->getTouchScreen()->getPoint();
    
        if (p.z > _controller->getTouchScreen()->pressureThreshhold) {
            p.x = map(p.x, TS_MINX, TS_MAXX, _controller->getScreenWidth(), 0);
            p.y = map(p.y, TS_MINY, TS_MAXY, _controller->getScreenHeight(), 0);
            item = checkForHit(p.x , p.y);
        }*/
		int length = 0;
		while(1 && _labels!=NULL){
			if(strcmp(_labels[length].getText(),"ENDOFFORM")==0)
				break;
			if(_labels[length].process())
				return &(_labels[length]);
			length++;
		}

		length = 0;
		while(1 && _buttons!=NULL){
			if(strcmp(_buttons[length].getText(),"ENDOFFORM")==0)
				break;
			if(_buttons[length].process())
				return &(_buttons[length]);
			length++;
		}

		length = 0;
		while(1 && _circleButtons!=NULL){
			if(strcmp(_circleButtons[length].getText(),"ENDOFFORM")==0)
				break;
			if(_circleButtons[length].process())
				return &(_circleButtons[length]);
			length++;
		}

		length = 0;
		while(1 && _arrowButtons!=NULL){
			if(strcmp(_arrowButtons[length].getText(),"ENDOFFORM")==0)
				break;
			if(_arrowButtons[length].process())
				return &(_arrowButtons[length]);
			length++;
		}

		length = 0;
		while(1 && _sliders!=NULL){
			if(strcmp(_sliders[length].getText(),"ENDOFFORM")==0)
				break;
			if(_sliders[length].process())
				return &(_sliders[length]);
			length++;
		}

	}while(waitForItem);
    
    return NULL;
}

TouchScreenArea *TouchScreenForm::checkForHit(unsigned int tx, unsigned int ty){
    int length = 0;
    while(1 && _labels!=NULL){
        if(strcmp(_labels[length].getText(),"ENDOFFORM")==0)
            break;
		if(_labels[length].checkForHit(tx,ty))
			return &(_labels[length]);
        length++;
    }

    length = 0;
    while(1 && _buttons!=NULL){
        if(strcmp(_buttons[length].getText(),"ENDOFFORM")==0)
            break;
		if(_buttons[length].checkForHit(tx,ty))
			return &(_buttons[length]);
        length++;
    }

    length = 0;
    while(1 && _circleButtons!=NULL){
        if(strcmp(_circleButtons[length].getText(),"ENDOFFORM")==0)
            break;
		if(_circleButtons[length].checkForHit(tx,ty))
			return &(_circleButtons[length]);
        length++;
    }

    length = 0;
    while(1 && _arrowButtons!=NULL){
        if(strcmp(_arrowButtons[length].getText(),"ENDOFFORM")==0)
            break;
		if(_arrowButtons[length].checkForHit(tx,ty))
			return &(_arrowButtons[length]);
        length++;
    }

    length = 0;
    while(1 && _sliders!=NULL){
        if(strcmp(_sliders[length].getText(),"ENDOFFORM")==0)
            break;
		if(_sliders[length].checkForHit(tx,ty))
			return &(_sliders[length]);
        length++;
    }

    return NULL;
}

void TouchScreenForm::setLabels(TouchScreenLabel widgets[]){
	_labels = widgets;
}

void TouchScreenForm::setButtons(TouchScreenButton widgets[]){
	_buttons = widgets;
}

void TouchScreenForm::setCircleButtons(TouchScreenCircleButton widgets[]){
	_circleButtons = widgets;
}

void TouchScreenForm::setArrowButtons(TouchScreenArrowButton widgets[]){
	_arrowButtons = widgets;
}

void TouchScreenForm::setSliders(TouchScreenSlider widgets[]){
	_sliders = widgets;
}
