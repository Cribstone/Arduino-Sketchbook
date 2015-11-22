/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 * 
 * DESCRIPTION
 * Example sketch showing how to request time from controller which is stored in RTC module
 * The time and temperature (DS3231/DS3232) is shown on an attached Crystal LCD display
 * 
 *
 * Wiring (radio wiring on www.mysensors.org)
 * ------------------------------------
 * Arduino   RTC-Module     I2C Display 
 * ------------------------------------
 * GND       GND            GND
 * +5V       VCC            VCC
 * A4        SDA            SDA
 * A5        SCL            SCL
 *
 * http://www.mysensors.org/build/display
 *
 */


#include <SPI.h>
#include <MySensor.h>  
#include <Time.h>  
#include <DS3232RTC.h>  // A  DS3231/DS3232 library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

MySensor gw;
boolean timeReceived = false;
unsigned long lastUpdate=0, lastRequest=0;

// Initialize display. Google the correct settings for your display. 
// The follwoing setting should work for the recommended display in the MySensors "shop".
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup()  
{  
  gw.begin();

  // Send the sketch version information to the gateway and Controller
  gw.sendSketchInfo("RTC Clock", "1.0");
  
  // the function to get the time from the RTC
  setSyncProvider(RTC.get);  

  // Request latest time from controller at startup
  gw.requestTime(receiveTime);  
  
  // initialize the lcd for 16 chars 2 lines and turn on backlight
  lcd.begin(16,2); 
}

// This is called when a new time value was received
void receiveTime(unsigned long controllerTime) {
  // Ok, set incoming time 
  Serial.print("Time value received: ");
  Serial.println(controllerTime);
  RTC.set(controllerTime); // this sets the RTC to the time from controller - which we do want periodically
  timeReceived = true;
}
 
void loop()     
{     
  unsigned long now = millis();
  gw.process();
  
  // If no time has been received yet, request it every 10 second from controller
  // When time has been received, request update every hour
  if ((!timeReceived && (now-lastRequest) > (10UL*1000UL))
    || (timeReceived && (now-lastRequest) > (60UL*1000UL*60UL))) {
    // Request time from controller. 
    Serial.println("requesting time");
    gw.requestTime(receiveTime);  
    lastRequest = now;
  }
  
  // Update display every second
  if (now-lastUpdate > 1000) {
    updateDisplay();  
    lastUpdate = now;
  }
}


void updateDisplay(){
  tmElements_t tm;
  RTC.read(tm);

  // Print date and time 
  lcd.home();
  lcd.print(tm.Day);
  lcd.print("/");
  lcd.print(tm.Month);
//  lcd.print(" ");
//  lcd.print(tmYearToCalendar(tm.Year)-2000);

  lcd.print(" ");
  printDigits(tm.Hour);
  lcd.print(":");
  printDigits(tm.Minute);
  lcd.print(":");
  printDigits(tm.Second);

  // Go to next line and print temperature
  lcd.setCursor ( 0, 1 );  
  lcd.print("Temp: ");
  lcd.print(RTC.temperature()/4);
  lcd.write(223); // Degree-sign
  lcd.print("C");
}


void printDigits(int digits){
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}


