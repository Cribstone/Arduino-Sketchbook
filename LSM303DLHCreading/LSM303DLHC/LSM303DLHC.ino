/*
 *
 * This sketch takes the readings from the compass and accelerometer LSM303DLHC
 * breakout board from adafruit and prints it to the serial port. It also uses
 * the libraries developed for this sensor by adafruit.
 *
 * It also uses an adapted smoothing method, smoothing all 6 axis from the
 * LSM303 sensor. It puts several readings in an array and calculates the
 * average.
 */

//https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout/calibration
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>



Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

float xmax = 0;
float x = 0;
const float earthGravity = 9.78;
const float compassAttraction = 60.0;

const int numberOfAxis = 6;
const int numberOfReadings = 36; // number of readings for the smoothing.
float readings[numberOfAxis];
float readingSums[numberOfAxis];
float sreadings[numberOfAxis][numberOfReadings];

int index = 0;

// readings from the calibration run. Using the calibration sketch from
// Adafruit  to find these. You should create your own readings that represent
// your own sensor.
//
//https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout/calibration
//
// the order of the readings stored in the array:
// acc x, y z, mag x, y, z:
// calibration run lowest readings.
float minReadings[numberOfAxis] = 
  {-13.45, -13.18, -10.83, -43.27, -50.0, -56.12};
// calibration run highest readings.
float maxReadings[numberOfAxis] = 
  {13.61, 11.96, 11.34, 57.09, 49.73, 52.04};



void setup (void) {
  Serial.begin(115200);
  if (!mag.begin()) { Serial.println("w: mag"); }
  if (!accel.begin()) { Serial.println("w: accel"); }
  if (!accel.begin() || !mag.begin()) {
    Serial.println("din't work out, did it?");
    while(1);
  }
} 



void loop(void) {
  sensors_event_t accevent;
  sensors_event_t magevent;
  accel.getEvent(&accevent);
  mag.getEvent(&magevent);


  readings[0] = accevent.acceleration.x;
  readings[1] = accevent.acceleration.y;
  readings[2] = accevent.acceleration.z;
  readings[3] = magevent.magnetic.x;
  readings[4] = magevent.magnetic.y;
  readings[5] = magevent.magnetic.z;
  populateReadings();
  printReadings();
}


/*
float map (int lindex, float multiplier) {
  return reading(lindex);
}
*/

float map (int lindex, float multiplier) {
  return map(reading(lindex), minReadings[lindex], maxReadings[lindex],
    -multiplier, multiplier);
}



float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



/*
 * Prints out the readings to the serial port.
 */
void printReadings() {

  Serial.print(map(0, earthGravity));
  Serial.print(":");
  Serial.print(map(1, earthGravity));
  Serial.print(":");
  Serial.print(map(2, earthGravity));
  Serial.print(":");
  Serial.print(map(3, compassAttraction));
  Serial.print(":");
  Serial.print(map(4, compassAttraction));
  Serial.print(":");
  Serial.print(map(5, compassAttraction));
  Serial.println();
}



/*
 * Stores the readings in an array to make it possible to calculate an average.
 */
float populateReadings() {
  for (int i = 0; i < numberOfAxis; i++) {
    sreadings[i][index] = readings[i];
    if (index == numberOfReadings - 1) {
      readingSums[i] -= sreadings[i][0];
    }
    else {
      readingSums[i] -= sreadings[i][index + 1];
    }
    readingSums[i] += readings[i];
  }
  if (index++ >= numberOfReadings - 1) { 
    index = 0; 
  }
}



float reading(int lindex) {
  return readingSums[lindex]/numberOfReadings;
}