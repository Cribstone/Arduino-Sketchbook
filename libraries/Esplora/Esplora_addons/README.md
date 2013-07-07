Esplora-Sketches
----------------
All the sketches related to the Arduino Esplora.

Esplora-BoxProcDemo
-------------------
Contains the *.ino for the Esplora that takes the Acceleromiter information and sends it to the pc over serial. It then uses that information in the Processing program that is there. It rotates the cube in the window based on the way you tilt the Esplora.

Esplora-Uno-StartSelect
-----------------------
Folder that holds the code for both the Uno and the Esplora. The test *.ino is to test if your Esplora is reading the Serial info from the Uno by lighting up the LED. The other *.ino for the Esplora is a modified version of my SNES9XController code but instead of remapping all the buttons on the click of the joystick it only remaps the top two and uses the two added buttons as start and select. The final *.ino is for the Uno to read the buttons and serial print the info.

Esplora4Emulator
----------------
Contains a handful of folders. Each folder contains an *.ino for the Esplora that sets it up for that specific emulator using the default control setup.

EsploraMouse
------------
The project that makes the Esplora be used as your mouse. You use the joystick to move the cursor and the buttons for clicking. Left is the left click, bottom is the middle click, and right is right click. The top is to toggle if the mouse is "on"

EsploraMouse2 
-------------
Exactly the same as EsploraMouse except instead of using the joystick it uses the Accelerometer.
