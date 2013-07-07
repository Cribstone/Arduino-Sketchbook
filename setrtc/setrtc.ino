#include <Wire.h>

#include "Time.h"
#include "DS1307RTC.h"


void setup()
{
     Serial.begin(9600);

  // the next two lines can be removed if the RTC has been set

  //  setTime(6,51,0,6,8,10); // set time to 17:05:00  1 Mar 2010 (see below)
  //  RTC.set(now());  // set the RTC to the current time (set in the previous line)

  // format for setting time - setTime(hr,min,sec,day,month,yr);

  setSyncProvider(RTC.get);   // the function to get the time from the RTC

}


void loop()
{
   Serial.print( "The time is ");

  Serial.print( year() );
   Serial.write('/');

   Serial.print( month() );
   Serial.write('/');

   Serial.print( day() );
   Serial.write(' ');

   Serial.print( hour() );
   Serial.write(':');

   Serial.print( minute() );
   Serial.write(':');

   Serial.println( second() );
         

}
