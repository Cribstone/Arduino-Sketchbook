// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

//the clock library object
RTC_DS1307 rtc;

//mini structure to handle debouncing buttons
struct DebounceButton
{
  int pin;
  int debounce;
  int prev_unstable_state;
  
  int state;
  int prev_state;
  int last_held_for;
  
  int isclicked() { return state==HIGH; }
  int isjustclicked() { return state==HIGH && prev_state==LOW; }
  int isjustunclicked() { return state==LOW && prev_state==HIGH; }
  
  DebounceButton(int _pin) {pin = _pin; debounce=0; state=LOW; prev_state=LOW; prev_unstable_state=LOW;}

  void update() 
  { 
    //store previous state
    prev_state = state;
    
    //read the new state
    int new_state = digitalRead(pin);

    //if we're stable, store the new state
    if(debounce == 0)
      state = new_state;
    
    //track how long we've been stable for, to avoid responding to button noise
    if(new_state != prev_unstable_state)
      debounce = 10; 
    else if(debounce>0) 
      debounce--;
    prev_unstable_state = new_state;
      
    //if down, track how long for
    if(isjustclicked())
      last_held_for=0;
    if(isclicked())
      last_held_for++;    
  }

};

uint32_t next_alarm_time = 0; //time alarm will next go off
uint32_t alarm_activate_time = 0; //time alarm activated (0 if not active)
DateTime now; //current date time (updated each loop)
uint32_t now_seconds; //current date time in seconds (cached from 'now')
float current_alarm_brightness = 0; //brightness of alarm effect
uint8_t left_light_brightness = 0;
uint8_t right_light_brightness = 0;
uint8_t left_light_state = 0;
uint8_t right_light_state = 0;
DebounceButton left_button(11);
DebounceButton right_button(12);
DebounceButton onoff_button(13);
uint8_t is_on = 1;
uint8_t on_effect = 0;
uint8_t off_effect = 0;

uint32_t current_left_col = 0;
uint32_t current_right_col = 0;

#define ALARM_TIME 8
#define ALARM_LENGTH (60*60)
#define SNOOZE_LENGTH (10*60)
#define ALARM_FADE_RATE (0.01f/(1.f*60.f))
//#define SNOOZE_LENGTH (20)
//#define ALARM_FADE_RATE (0.01f/(0.25f*60.f))

Adafruit_NeoPixel strip = Adafruit_NeoPixel(86, 6, NEO_GRB + NEO_KHZ800);

int countdown_it = 0;
void print_countddown()
{
    countdown_it = (countdown_it+1)%100;
    if(countdown_it == 0 && next_alarm_time)
    {
    Serial.print("Time is: ");
    Serial.print(now.hour(),DEC);
    Serial.print(":");
    Serial.print(now.minute(),DEC);
    Serial.print(":");
    Serial.print(now.second(),DEC);
    Serial.println();
    
  
      TimeSpan t; 
      if(alarm_activate_time > 0)
      {
        Serial.print("Alarm is active, ends in: ");
        t = TimeSpan((20*60) - (now_seconds-alarm_activate_time));
      }
      else
      {
        Serial.print("Next alarm in: ");
        t = TimeSpan(next_alarm_time-now_seconds);
      }
      Serial.print(t.days(),DEC);
      Serial.print(" days, ");
      Serial.print(t.hours(),DEC);
      Serial.print(" hours, ");
      Serial.print(t.minutes(),DEC);
      Serial.print(" minutes, ");
      Serial.print(t.seconds(),DEC);
      Serial.print(" seconds");
      Serial.println();        
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void setup () {
  
  //init the strip and call show (which updates all pixels, currently set to 'off')
  strip.begin();
  strip.show(); 
  
  //init serial, i2c coms and the real time clock
  Serial.begin(57600);
  Wire.begin();
  rtc.begin();
    
  //always init the clock to 7:59:50, as we're going to have an alarm at 8
  //rtc.adjust(DateTime(F(__DATE__),F("07:59:50")));

  //resets time to compile time
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //adjust clock to match now if not been set yet
  
  if (! rtc.isrunning()) {
    Serial.println("RTC not running - resetting the clock");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  
  //init the button pins
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  
  Serial.print("Running light clock");
  Serial.println();
  
}

//resets the next alarm time to be the next 8AM
void reset_next_alarm_time()
{
    //get seconds since 1970
    uint32_t curr_time = now_seconds;
    
    //chop out the hours, minutes and seconds bit
    uint32_t day_start_time = (curr_time / 86400L) * 86400L;
    
    //add on 8 hours and also reset alarm active
    next_alarm_time = day_start_time + 3600L * ALARM_TIME;
    alarm_activate_time = 0;
    
    //move ahead if new alarm time is before now
    while(next_alarm_time < curr_time)
      next_alarm_time += 86400L;
}

//cancels the alarm. should eventually reset until next 8AM, but ust advance 30s for now
void cancel_alarm()
{
    Serial.print("Alarm off");
    Serial.println();
    //next_alarm_time = now_seconds + 30; //advance the next alarm time by 30s
    //alarm_activate_time = 0;
    reset_next_alarm_time(); //resets to go off at prev 8 AM
}

//snooze alarm. just turns it off and delays a few seconds
void snooze_alarm()
{
    Serial.print("Snooze");
    Serial.println();  
    alarm_activate_time = 0;
    next_alarm_time = now_seconds + SNOOZE_LENGTH;    
}

//updates the alarm (if active)
void update_alarm()
{
  //always deactivate alarm after fixed period of time or cancel pressed
  if( (now_seconds-alarm_activate_time) > ALARM_LENGTH || digitalRead(10))
  {
    cancel_alarm();
    return;
  }
   
  //check for snooze
  if(digitalRead(9))
  {
    //snoozing, so deactivate alarm and set it to go off in a little while
    snooze_alarm();
    return;
  }
}

void update_pixels()
{
  //choose target brightness
  float target_alarm_brightness = alarm_activate_time > 0 ? 1 : 0;
  
  //get and increment/decrement new brightness
  if(current_alarm_brightness < target_alarm_brightness)
    current_alarm_brightness = current_alarm_brightness + ALARM_FADE_RATE; //5 minutes to fade in
  else if(current_alarm_brightness > target_alarm_brightness)
    current_alarm_brightness = current_alarm_brightness - 0.01f;
    
  //clamp to within 0-1 range
  if(current_alarm_brightness < 0)
    current_alarm_brightness = 0;
  if(current_alarm_brightness > 1)
    current_alarm_brightness = 1;
   
 //force no alarm light if off
  float ab = current_alarm_brightness; 
  if(is_on == 0)
    ab = 0;
      
  //calculate target colours
  uint32_t new_left_col = ab==0 ? Adafruit_NeoPixel::Color(left_light_brightness,left_light_brightness/2,0) : Adafruit_NeoPixel::Color(0.1*ab*255,0.3*ab*255,ab*255);
  uint32_t new_right_col = ab == 0 ? Adafruit_NeoPixel::Color(right_light_brightness,right_light_brightness/2,0) : Adafruit_NeoPixel::Color(0.1*ab*255,0.3*ab*255,ab*255);
  
  if(on_effect > 0)
    new_left_col = new_right_col = (on_effect > 20 && on_effect < 80) ? Adafruit_NeoPixel::Color(0,50,0) : 0;
  else if(off_effect > 0)
    new_left_col = new_right_col = (off_effect > 20 && off_effect < 80) ? Adafruit_NeoPixel::Color(50,0,0) : 0;
  
  //if we have a change, update the strip
  if( (current_left_col != new_left_col || current_right_col != new_right_col) && strip.canShow())
  {
    //apply colour
    strip.clear();
    for(int i=0; i<strip.numPixels()/2; i++)
      strip.setPixelColor(i, new_left_col);
    for(int i=strip.numPixels()/2; i<strip.numPixels(); i++)
      strip.setPixelColor(i, new_right_col);
    
    strip.show();
    
    //store colour
    current_left_col = new_left_col;
    current_right_col = new_right_col;
  }
}

void update_reading_lights()
{
  left_button.update();
  right_button.update();
  
  if(left_button.isjustunclicked() && left_button.last_held_for < 50)
  {
      left_light_state = (left_light_state+1)%4;
      Serial.print("Left light clicked"); Serial.println();
  }
  else if(left_button.isclicked() && left_button.last_held_for == 50)
  {
      left_light_state = right_light_state = 0;
      Serial.print("Left light held"); Serial.println();
  }
  
  
  if(right_button.isjustunclicked() && right_button.last_held_for < 50)
  {
      right_light_state = (right_light_state+1)%4;
      Serial.print("Right light clicked"); Serial.println();
  }      
  else if(right_button.isclicked() && right_button.last_held_for == 50)
  {
      left_light_state = right_light_state = 0;
      Serial.print("Right light held"); Serial.println();
  }
 
  int target_left = left_light_state*13;
  if(left_light_brightness < target_left) left_light_brightness++;
  else if(left_light_brightness > target_left) left_light_brightness--;
  
  int target_right = right_light_state*13;
  if(right_light_brightness < target_right) right_light_brightness++;
  else if(right_light_brightness > target_right) right_light_brightness--;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  uint16_t i, j;

  Serial.print("Doing rainbow cycle...");

  j=0;
  while(true)
  { 
    onoff_button.update();
    
    if(onoff_button.isjustclicked())
      break;
    
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    j++;
    delay(1);
  }
  
  strip.clear();
  strip.show();
  
  while(onoff_button.isclicked())
    onoff_button.update();
  
  Serial.print(" Done!"); Serial.println();
}

void update_onoff()
{
  onoff_button.update();
 
  if(onoff_button.isjustunclicked() && onoff_button.last_held_for < 50)
  {
    Serial.print("On off button clicked"); Serial.println();
    is_on = 1-is_on;
    if(is_on)
    {
     on_effect = 100;
     off_effect = 0;
    }
    else
    {
      on_effect = 0;
     off_effect = 100; 
    }
  } 
  else if(onoff_button.isclicked() && onoff_button.last_held_for == 50)
  {
    rainbowCycle();
  }
  
  if(on_effect > 0)
    on_effect--;
  if(off_effect > 0)
    off_effect--;
}

void loop () {
  //always update 'now'
  now = rtc.now();
  now_seconds = now.unixtime();
  
  //if we don't have an alarm time, reset it to be the next 8AM
  if(next_alarm_time == 0)
    reset_next_alarm_time();
    
  //check if we've passed the alarm time
  if(now_seconds > next_alarm_time)
  {
    Serial.print("Alarm on");
    Serial.println();
    alarm_activate_time = now_seconds; //store the alarm is now active
    next_alarm_time += 1000000L; //advance the next alarm time by years!
  }
  
  //if alarm if active, do active alarm stuff!
  if(alarm_activate_time > 0)
    update_alarm();
    
  //do reading light logic
  update_reading_lights();
  
  //do on off button
  update_onoff();
    
  //refresh pixels
  update_pixels();
  
  print_countddown();
        
  delay(10);
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
