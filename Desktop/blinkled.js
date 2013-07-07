require('bonescript');
ledPin = bone.USR3;
setup = function() {
    pinMode(ledPin, OUTPUT);
};
loop = function() {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
};