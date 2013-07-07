
 


int buttonState[17];
int joystick[4];
int AnalogButton[2];

int inputCommand = 0;

void setup()
{
  Serial1.begin(9600);  //Init the Serial1 port to enable the xbee wireless communication
  InitIO();             //Initialize the inputs/outputs and the buffers
}

unsigned long timer = 0;

void loop()
{
  if(millis() - timer > 500){  // manage the updating freq of all the controlling information
    DataUpdate();  //read the buttons and the joysticks data
    printData();   //print the datas and states
    timer = millis(); 
  }
  
  if(Serial.available()){
    char input = Serial.read();
    
    switch(input){
      case 'w'://Move Forward
        Serial1.println("w");
        inputCommand = input;
        break;
      
      case 's'://Move Backwards
        Serial1.println("s");
        inputCommand = input;
        break;
        
        case 'a'://Turn Left
        Serial1.println("a");
        inputCommand = input;
        break;
          
        case 'd'://Turn Right  
        Serial1.println("d");
        inputCommand = input;
        break;
        
        case 'l'://LED
        Serial1.println("l");
        inputCommand = input;
        break;
        
        case 'f'://Stop
        Serial1.println("l");
        inputCommand = input;
        
      default:
        break;
    }
  }
}

void DataUpdate(){
  
  for(int i = 3; i < 17; i++)  buttonState[i] = digitalRead(i);
  buttonState[0] = analogRead(0);
  buttonState[1] = analogRead(1);
  for(int i = 0; i < 4; i++)  joystick[i] = analogRead(i);
  for(int i = 4; i < 6; i++)  AnalogButton[i-4] = analogRead(i);
  
}

String Buttons[17] = {
  "J2","J1",NULL,"S2","S1","UP","LEFT","DOWN","RIGHT","1","4","2","3","RZ1","RZ2","LZ1","LZ2"};
  // Buttons Nmes

void printData(){
//  for(int i = 0; i < 17; i++)  Serial.print(buttonState[i]),Serial.print(" ");
//  for(int i = 0; i < 4; i++)  Serial.print(joystick[i]),Serial.print(" ");
//  for(int i = 0; i < 2; i++)  Serial.print(AnalogButton[i]),Serial.print(" ");
//  Serial.println("");
  Serial1.print("Button Pressed:");
  for(int i = 0; i < 2; i++)  if(buttonState[i] < 100)  Serial.print(Buttons[i]),Serial.print(",");
  for(int i = 3; i < 17; i++)  if(buttonState[i] == 0)  Serial.print(Buttons[i]),Serial.print(",");
  Serial1.println("");
  Serial1.print("Analog Sticks:");
  for(int i = 0; i < 4; i++)  Serial.print(joystick[i]),Serial.print(",");
  for(int i = 0; i < 2; i++)  Serial.print(AnalogButton[i]),Serial.print(",");
  Serial1.println("");
  Serial1.println(inputCommand);
}  
