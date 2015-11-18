const int numberOfKeys = 7;
int keys[numberOfKeys] = { 6, 9, 10, 11, A2, A3, A4 };
int notes[numberOfKeys] = {1046, 1175, 1319, 1598, 1760, 2093, 2349};

int speaker = 5;
int touchValue;

void setup() {
pinMode(speaker, OUTPUT);
int i = 0;
while (i < numberOfKeys)	//set the piano keys to be inputs
{
pinMode(keys[i], INPUT);
i = i+1;
}
Serial.begin(9600);	// initialize the communication
}

void loop() {
int i = 0;
while (i < numberOfKeys)
{
checkPianoKey(keys[i], notes[i]);
i = i+1;
}
Serial.println();
delay(100);
}

void checkPianoKey(int key, int note) {
touchValue = readCapacitivePin(key); //read touch sensor value
Serial.print(touchValue);	//send value to the computer
Serial.print("\t");	//send a tab
if (touchValue > 1) //if the key is pressed
{
tone(speaker, note); // play a note
delay(100);	// wait for 1/10th of a second
}
else	// if the key is not pressed
{
noTone(speaker); // stop playing the note
}
}
