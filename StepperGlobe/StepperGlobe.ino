#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
void setup() {
    pinMode(IN1, OUTPUT); 
    pinMode(IN2, OUTPUT); 
    pinMode(IN3, OUTPUT); 
    pinMode(IN4, OUTPUT); 
}

uint64_t timeMultiplier = 24 * 60 * 1; // that gives you ~ one rotation per minute.

uint64_t  msPerDay = 24ULL * 3600ULL * 1000ULL; 
uint64_t  divideForStep = 64ULL * 48ULL * timeMultiplier;
double nextTimeStep = 0;

void loop() {
    static unsigned long zeroTime = millis();
    uint64_t curTime = millis() - zeroTime;
    if(curTime * divideForStep > nextTimeStep) {
        nextTimeStep += msPerDay;
        stepper(true); 
    }
}

inline void stepper(bool normalRotation) {
    static int Steps = 0;
    Steps += 8 + (normalRotation ? 1 : -1);
    Steps %= 8;
    switch(Steps) {
    case 0:
        digitalWrite(IN1, LOW); 
        break; 
    case 1:
        digitalWrite(IN3, HIGH);
        break; 
    case 2:
        digitalWrite(IN4, LOW);
        break; 
    case 3:
        digitalWrite(IN2, HIGH);
        break; 
    case 4:
        digitalWrite(IN3, LOW);
        break; 
    case 5:
        digitalWrite(IN1, HIGH); 
        break; 
    case 6:
        digitalWrite(IN2, LOW);
        break; 
    case 7:
        digitalWrite(IN4, HIGH);
        break; 
    default:
        break; 
    }
}


