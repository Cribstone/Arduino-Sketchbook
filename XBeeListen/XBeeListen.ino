char buffer;
uint8_t led = 13;

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop(){
  while(Serial.available()){
    buffer = Serial.read();
    if (buffer == 'H'){
      digitalWrite(led,HIGH);
    }else if (buffer == 'L'){
      digitalWrite(led,LOW);
    }
  }
}

int main (){
  setup();
  while(1){
    loop();
  }
  return (0);
}

