This is the TouchScreenMenu library version 1.2 for Arduino 1.0.1 IDE.  This library supports more than just menus.  There are menus, buttons, graphics functions, labels, etc.

This library currently uses the TFT and TouchScreen libraries that can be found at: http://www.seeedstudio.com/wiki/File:TFT_touch_1.0_Libraries.zip

Where possible the use of these libraries has been hidden in an attempt to make it easier to replace them with other libraries and to allow the use of other hardware.

_________________________________________________________________________________________________________________

Tested hardware:

MPU Board			Screen				URLs for Screen
_________________________________________________________________________________________________________________
Arduino UNO Rev 3			2.8" TFT Touch Shield (v1)	http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html?cPath=132_134
									http://www.seeedstudio.com/wiki/2.8%27%27_TFT_Touch_Shield_V1.0
									http://www.seeedstudio.com/wiki/File:TFT_touch_1.0_Libraries.zip
Arduino Mega			2.8" TFT Touch Shield (v1)	http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html?cPath=132_134
									http://www.seeedstudio.com/wiki/2.8%27%27_TFT_Touch_Shield_V1.0
									http://www.seeedstudio.com/wiki/File:TFT_touch_1.0_Libraries.zip
SainSmart Mega			2.8" TFT Touch Shield (v1)	http://www.seeedstudio.com/depot/28-tft-touch-shield-p-864.html?cPath=132_134
									http://www.seeedstudio.com/wiki/2.8%27%27_TFT_Touch_Shield_V1.0
									http://www.seeedstudio.com/wiki/File:TFT_touch_1.0_Libraries.zip

_________________________________________________________________________________________________________________
Changes:
12/21/2012 - Scott Shaver
	Added the example TouchScreenFormExample which uses the new TouchScreenForm class.
01/15/2013
	Fixed known bugs (mainly those missing intialization lines in the constructors) and added another partial example of a real application.
_________________________________________________________________________________________________________________


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
