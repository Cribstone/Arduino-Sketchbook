//Stupid Pet Trick Project
// Kaitlin Till-Landry & Engin Ayaz
// NYU ITP Sep 2011
// Physical Computing Assignment// initialize two patches, this can go up to 3 in the future
int patch0 = 0;
int patch1 = 0;// modify these values depending on patch and touch sensitivity
int patch0Threshold = 410;
int patch1Threshold = 410;boolean switch1 = false;
boolean switch2 = false;int timeLag = 2200; // implies almost two seconds lag to start or stop the player// location of digital output in arduino
int musicSwitch = 7;// to hold the value of O or 1 for the activation signal
int musicState;// Initialize the timer
long timer = 0;void setup() {
Serial.begin(9600);
pinMode(musicSwitch, OUTPUT);
}void loop(){
patch0 = analogRead(A0);
patch1 = analogRead(A1);
musicState = digitalRead(musicSwitch);Serial.println(patch1);// if either patch is above threshold, turn switch 1 on
if(patch0 > patch0Threshold){
switch1 = true;
timer = millis();
}// If “now” is less than two seconds from the activation of the patch
// turn the other switch true
if(millis()-timer<timeLag){
if(patch1>patch1Threshold){
switch2 = true;
}
}else{
switch1 = false;
switch2 = false;
}// play only when both switches are true.
if(switch1 == true && switch2 == true){
digitalWrite(musicSwitch, HIGH);
}// stop playing when someone stops touching it
// to stop, one needs to send a low and high signal to the radioshack 9V player
else{
digitalWrite(musicSwitch, LOW);
digitalWrite(musicSwitch, HIGH);
}
}
