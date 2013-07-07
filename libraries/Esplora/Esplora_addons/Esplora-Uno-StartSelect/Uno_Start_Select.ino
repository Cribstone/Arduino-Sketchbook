const int selectBtn = 12;
const int startBtn = 13;

boolean lastbtnStates[2];
const byte Buttons[2] = {selectBtn, startBtn};

void setup(){
  Serial.begin(9400);
  pinMode(selectBtn, INPUT);
  pinMode(startBtn, INPUT);
}

void loop(){
  
  for (byte Button = 0; Button <= 1; Button++) {  
    boolean newState = digitalRead(Buttons[Button]);
    if (newState != lastbtnStates[Button]){
      if (newState == HIGH){
        if (Button == 0){
          Serial.print("Select");
          Serial.print(';');
        }
        else{
          Serial.print("Start");
          Serial.print(';');
        }
      }
    }
    lastbtnStates[Button] = newState;
  }
  delay(50);
}

  
