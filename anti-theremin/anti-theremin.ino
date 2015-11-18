Here is the sketch

// Optical Theramin

//pin definitions
#define PHONES 9   // headphones connected to digital pin 9
#define PHOTOCELL 0 //photocell analog in pin 0

//variable definitions
long val = 0;        //stores raw value from photocell
long maxread = 0;    //maximum value from calibration phase
long minread = 1000;  // minimum value from calibration phase
double f = 0;         // frequency of sound
double normf = 0;    // normalized frequency 
double logf = 0;      // logarithm of normalized frequency
int ilogf = 0;        // rounded logarithm
int i = 0;            // loop dummy variable
double factor = 0;    // scaling factor for calibration
double shift = 0;     // shift for calibration
long maxfreq = 1048;  // maximum desired frequency after calibration
long minfreq = 131;   // minimum desired frequency after calibration


//magic numbers that make the intervals sound pleasing
//double gap = 1.148698355;  //ratio of consecutive notes (pentatonic)
                             // it's the 5th root of 2
double gap = 1.059463094;  //ratio of consecutive notes (chromatic)
                              // its the 12th root of 2

                              
void setup()
{
  pinMode(PHONES, OUTPUT);    // sets the digital pin as output

// calibration loop to determine a rasonable range of light levels (minread to maxread)
// and map that to frequencies between minfreq and maxfreq
  for (i = 0; i< 500; i++) {  // calibration loop runs for 5 seconds
    val = analogRead(PHOTOCELL);   // read photocell 
    tone(PHONES, val);          // play raw tone to guide calibration
    if (val > maxread) {        // as the values climb, store the largest
      maxread = val;
    }
    if (val < minread) {        // as the values drop, store the smallest
      minread = val;
    }  
    delay(10);                  // reasonable delay
  } 
  
  //Now we use the calibration to calculate scale and shift parameters
  factor = (double)(maxfreq - minfreq) / (double)(maxread - minread); // scale parameter
  //it's like a slope
  
  shift = factor * minread - minfreq;  //shift parameter: it's like an offset
}

void loop()
{
  val = analogRead(PHOTOCELL);   // read photocell 
  f = factor * val - shift;     // this linearly maps the frequency to
                                // a value between minfreq and maxfreq
                                // according to the calibration result
  
  normf = f / (double) minfreq;  // Dividing an exponential function by the min value
  logf = log(normf) / log(gap); // allows us to take the log (base gap) and the result 
  ilogf = round(logf);           // is the number of notes above the lowest, once we round it.
  f = minfreq * pow(gap,ilogf);  // we better "unlog" it.
  
  
  tone(PHONES, f);              // this produces the tone signal
  
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
