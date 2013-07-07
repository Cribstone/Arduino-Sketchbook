#define FANPIN 5

int state = LOW;     // note this is a global variable IMPORTANT
int setpoint = 20;
int threshold = 5;

void setup()
{
  Serial.begin(115200);
  Serial.println("Start...");
  pinMode(FANPIN, OUTPUT);
}
  

void loop()
{
  float t = readTemperature();

  if (t > setpoint+threshold)
  {
    state = ON;
  }
  if (t < setpoint)
  {
    state = OFF;
  }

  Serial.print("temp: ");
  Serial.print(t, 2);              // 2 decimals
  Serial.print("\t\tstate: ");
  Serial.print(state);

  setFan(state);
}

void setFan(int state)
{
  digitalWrite(FANPIN, state);
}

float readTemperature()
{
  float t = analogRead(A0);   // assuming an analog LM35 sensor , other code
  t = t * 5000 / 1023;           // make millivolts of it
  t = -25 + t/10;                  // 1C per 10 mv  (dont know exact formula)
}
