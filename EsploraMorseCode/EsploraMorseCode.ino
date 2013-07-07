/*
Esplora Morse Code

Makes the light sensor into a Morse code sender. 
Cover the sensor for a short time to make a dit/dot; cover for a longer time to make dah/dash.

The buzzer makes a tone when the light sensor is covered, so you can hear the dits and dahs.

Letters are sent over serial.

Press UP button to recalibrate the light sensor.
Press LEFT button to send the dots and dashes over serial (as well as the letters).
Press RIGHT button to not send the dots and dashes over serial (only send the letters).

Send messages over serial and the buzzer will play them back in Morse code.

*/

#include <Esplora.h>

int light_threshold, freq;
boolean calibrated;
double on_time, off_time;
boolean letter_printed, space_printed;
boolean show_dits;

boolean new_line;

const int dit_threshold = 150; //how long a DIT is considered to be, in milliseconds
const int dash_threshold = 3*dit_threshold; //Official length is 3*dit
const int letter_threshold = 3*dit_threshold; //Official length is 3*dit
const int word_threshold = 7*dit_threshold; //Official length is 3*dit

const int DIT_LENGTH = 100;
const int DASH_LENGTH = 3*DIT_LENGTH;
const int LETTER_LENGTH = 3*DIT_LENGTH;
const int WORD_LENGTH = 7*DIT_LENGTH;

const int DASH = 1, DOT = 0;
int code[5];
int length;

void setup() {
  
  Serial.begin(9600);
  
  light_threshold = 512;
  freq = 440;
  calibrated = false;
  on_time = 0;
  length = 0;
  letter_printed = false;
  space_printed = false;
  show_dits = false;
  new_line = true;
}

void loop() {
  
    //Press down to recalibrate
  if (Esplora.readButton(SWITCH_DOWN) == LOW) {
    calibrated = false;
  }
  if (!calibrated) {
    calibrated = calibrate();
  }
  
  if (Esplora.readLightSensor() < light_threshold) { //The sensor is covered
    Esplora.tone(freq);
    
    if (!new_line) {
      Serial.println();
      new_line = true;
    }
    
    if (on_time <= 0) {
      on_time = millis();
    }

    off_time = 0;    
    space_printed = false;
    letter_printed = false;
    
  } else { //The sensor is not covered
    Esplora.noTone();
    
    if (off_time <= 0) {
      off_time = millis();
    }
    
    if (off_time > 0 && millis() - off_time > word_threshold  && !space_printed) {
      Serial.print(' ');
      space_printed = true;
    }
    
    if (off_time > 0 && millis() - off_time > letter_threshold && !letter_printed) {
      print_letter(code, length);
      length = 0;
      letter_printed = true;
    }
    
    if (on_time > 0 && millis() - on_time > dit_threshold) {
      if (show_dits) {
        Serial.print("-");
      }
      code[length] = DASH;
      length = (length + 1) % 5;
    } else if (on_time > 0 && millis() - on_time > 0) {
      if (show_dits) {
        Serial.print (".");
      }
      code[length] = DOT;
      length = (length + 1) % 5;
    } 
    on_time = 0;
  }
  
  //Press LEFT button to show the dots and dashes
  if (Esplora.readButton(SWITCH_LEFT) == LOW) {
    show_dits = true;
  }
  
  //Press RIGTH button to turn off the dots and dashes
  if (Esplora.readButton(SWITCH_RIGHT) == LOW) {
    show_dits = false;
  }
  
  while (Serial.available() > 0) {
    
    if (new_line) {
      Serial.println();
      new_line = false;
    }
    
    char c = Serial.read();
    output_letter(c);
  }
  
  delay(10);
}

boolean calibrate() {
 
 int start = millis();
 light_threshold = Esplora.readLightSensor();
 while (millis() - start < 100) {
   light_threshold = (light_threshold + Esplora.readLightSensor()) / 2;
 }
 light_threshold -= 80;
 return true;
}


void print_letter(int c[], int len) { 
 char letter = ' ';  
 switch (len) {
  case 1:
  
  if (c[0] == DOT) {
    letter = 'E';
  } else if (c[0] == DASH) {
    letter = 'T';
  }
  
  break;
  
  case 2:
  
  if (c[0] == DOT && c[1] == DOT) {
    letter = 'I';
  } else if (c[0] == DOT && c[1] == DASH) {
   letter = 'A';
  } else if (c[0] == DASH && c[1] == DOT) {
   letter = 'N';
  } else if (c[0] == DASH && c[1] == DASH) {
   letter = 'M';
  } 
  break;
  
  case 3:
  
  if (c[0] == DOT && c[1] == DOT && c[2] == DOT) {
    letter = 'S';    
  } else if (c[0] == DOT && c[1] == DOT && c[2] == DASH) {
    letter = 'U'; 
  } else if (c[0] == DOT && c[1] == DASH && c[2] == DOT) {
    letter = 'R'; 
  } else if (c[0] == DOT && c[1] == DASH && c[2] == DASH) {
    letter = 'W'; 
  } else if (c[0] == DASH && c[1] == DOT && c[2] == DOT) {
    letter = 'D'; 
  } else if (c[0] == DASH && c[1] == DOT && c[2] == DASH) {
    letter = 'K'; 
  } else if (c[0] == DASH && c[1] == DASH && c[2] == DOT) {
    letter = 'G'; 
  } else if (c[0] == DASH && c[1] == DASH && c[2] == DASH) {
    letter = 'O'; 
  }
  
  break;
 
  case 4:
  
  if (c[0] == DOT && c[1] == DOT  && c[2] == DOT && c[3] == DOT) {
    letter = 'H';
  } else if (c[0] == DOT && c[1] == DOT  && c[2] == DOT && c[3] == DASH) {
    letter = 'V';
  } else if (c[0] == DOT && c[1] == DOT  && c[2] == DASH && c[3] == DOT) {
    letter = 'F';
  } else if (c[0] == DASH && c[1] == DOT  && c[2] == DOT && c[3] == DOT) {
    letter = 'B';
  } else if (c[0] == DASH && c[1] == DOT  && c[2] == DASH && c[3] == DOT) {
    letter = 'C';
  } else if (c[0] == DOT && c[1] == DASH  && c[2] == DASH && c[3] == DASH) {
    letter = 'J';
  } else if (c[0] == DOT && c[1] == DASH  && c[2] == DOT && c[3] == DOT) {
    letter = 'L';
  } else if (c[0] == DOT && c[1] == DASH  && c[2] == DASH && c[3] == DOT) {
    letter = 'P';
  } else if (c[0] == DASH && c[1] == DASH  && c[2] == DOT && c[3] == DASH) {
    letter = 'Q';
  } else if (c[0] == DASH && c[1] == DOT  && c[2] == DOT && c[3] == DASH) {
    letter = 'X';
  } else if (c[0] == DASH && c[1] == DOT  && c[2] == DASH && c[3] == DASH) {
    letter = 'Y';
  } else if (c[0] == DASH && c[1] == DASH  && c[2] == DOT && c[3] == DOT) {
    letter = 'Z';
  }  
  break;
 
  case 0: {
  
    int i;
    int val = 0;
    boolean large = (code[0] == DASH);
    for (int i = 0; i < 5; i++) {
      if (code[i] == DOT) {
        val += 1;
      } else if (large) {
        val += 2;
      }        
    }
    val = val % 10;
    letter = '0' + val; 
  }
  break;
 
  default:
   letter = '?';    
 }
 
 Serial.print(letter);  
}

void output_letter(char c) {
 
 switch (c) {
   case 'A':
   case 'a':
     beep(DOT);
     beep(DASH);
   break;
   case 'B':
   case 'b':
     beep(DASH);
     beep(DOT);
     beep(DOT);
     beep(DOT);
   break;
   case 'C':
   case 'c':   
     beep(DASH);
     beep(DOT);
     beep(DASH);
     beep(DOT);
   break;
   case 'D':
   case 'd':
       beep(DASH);
       beep(DOT);
       beep(DOT);
   break;
   case 'E':
   case 'e':
       beep(DOT);
   break;
   case 'F':
   case 'f':
       beep(DOT);
       beep(DOT);
       beep(DASH);
       beep(DOT);
   break;
   case 'G':
   case 'g':
       beep(DASH);
       beep(DASH);
       beep(DOT);
   break;
   case 'H':
   case 'h':
       beep(DOT);
       beep(DOT);
       beep(DOT);
       beep(DOT);
   break;
   case 'I':
   case 'i':
       beep(DOT);
       beep(DOT);
   break;
   case 'J':
   case 'j':
       beep(DOT);
       beep(DASH);
       beep(DASH);
       beep(DASH);
   break;
   case 'K':
   case 'k':
       beep(DASH);
       beep(DOT);
       beep(DASH);
   break;
   case 'L':
   case 'l':
       beep(DOT);
       beep(DASH);
       beep(DOT);
       beep(DOT);
   break;
   case 'M':
   case 'm':
       beep(DASH);
       beep(DASH);
   break;
   case 'N':
   case 'n':
       beep(DASH);
       beep(DOT);
   break;
   case 'O':
   case 'o':
       beep(DASH);
       beep(DASH);
       beep(DASH);
   break;
   case 'P':
   case 'p':
       beep(DOT);
       beep(DASH);
       beep(DASH);
       beep(DOT);
   break;
   case 'Q':
   case 'q':
       beep(DASH);
       beep(DASH);
       beep(DOT);
       beep(DASH);
   break;
   case 'R':
   case 'r':
       beep(DOT);
       beep(DASH);
       beep(DOT);
   break;
   case 'S':
   case 's':
       beep(DOT);
       beep(DOT);
       beep(DOT);
   break;
   case 'T':
   case 't':
       beep(DASH);
   break;
   case 'U':
   case 'u':
       beep(DOT);
       beep(DOT);
       beep(DASH);
   break;
   case 'V':
   case 'v':
       beep(DOT);
       beep(DOT);
       beep(DOT);
       beep(DASH);
   break;
   case 'W':
   case 'w':
       beep(DOT);
       beep(DASH);
       beep(DASH);
   break;
   case 'X':
   case 'x':
       beep(DASH);
       beep(DOT);
       beep(DOT);
       beep(DASH);
   break;
   case 'Y':
   case 'y':
       beep(DASH);
       beep(DOT);
       beep(DASH);
       beep(DASH);
   break;
   case 'Z':
   case 'z':
       beep(DASH);
       beep(DASH);
       beep(DOT);
       beep(DOT);
   break;
   
   case '1':
     beep(DOT);
     beep(DASH);
     beep(DASH);
     beep(DASH);
     beep(DASH);
   break;
   case '2':
     beep(DOT);
     beep(DOT);
     beep(DASH);
     beep(DASH);
     beep(DASH);
   break;
   case '3':
     beep(DOT);
     beep(DOT);
     beep(DOT);
     beep(DASH);
     beep(DASH);
   break;
   case '4':
     beep(DOT);
     beep(DOT);
     beep(DOT);
     beep(DOT);
     beep(DASH);
   break;
   case '5':
     beep(DOT);
     beep(DOT);
     beep(DOT);
     beep(DOT);
     beep(DOT);
   break;
   case '6':
     beep(DASH);
     beep(DOT);
     beep(DOT);
     beep(DOT);
     beep(DOT);
   break;
   case '7':
     beep(DASH);
     beep(DASH);
     beep(DOT);
     beep(DOT);
     beep(DOT);
   break;
   case '8':
     beep(DASH);
     beep(DASH);
     beep(DASH);
     beep(DOT);
     beep(DOT);
   break;
   case '9':
     beep(DASH);
     beep(DASH);
     beep(DASH);
     beep(DASH);
     beep(DOT);
   break;
   case '0':
     beep(DASH);
     beep(DASH);
     beep(DASH);
     beep(DASH);
     beep(DASH);     
   break;
   
   case ' ':
     delay(WORD_LENGTH - LETTER_LENGTH);
     Serial.print("   ");
 }
 
 delay(LETTER_LENGTH);
 Serial.print(" ");
}

void beep(int d) {
  
  switch (d) {
    case DOT:
     Serial.print(".");
     Esplora.tone(freq, DIT_LENGTH);
