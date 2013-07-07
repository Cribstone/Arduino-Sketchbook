//Xbee receiver
//This example code is in the Public Domain
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
int backLight = 13;    // pin 13 will control the backlight

OneWire  ds(8);        // ds18b20 pin #2 (middle pin) to Arduino pin 8

byte i;
byte present = 0;
byte data[12];
byte addr[8];
  
int HighByte, LowByte, SignBit, Whole, Fract, TReading, Tc_100, FWhole;

void setup(void) {
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.
  lcd.begin(2,16);              // rows, columns.  use 2,16 for a 2x16 LCD, etc.
  lcd.clear();                  // start with a blank screen
  lcd.setCursor(0,0);           // set cursor to column 0, row 0
  
    if ( !ds.search(addr)) {
      lcd.clear(); lcd.print("No more addrs");
      delay(1000);
      ds.reset_search();
      return;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      lcd.clear(); lcd.print("CRC not valid!");
      delay(1000);
      return;
  }
}

void getTemp() {
  int foo, bar;
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
  
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  
  if (SignBit) {
    TReading = -TReading;
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  Whole = Tc_100 / 100;          // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
  if (Fract > 49) {
    if (SignBit) {
      --Whole;
    } else {
      ++Whole;
    }
  }

  if (SignBit) {
    bar = -1;
  } else {
    bar = 1;
  }
  foo = ((Whole * bar) * 18);      // celsius to fahrenheit conversion section
  FWhole = (((Whole * bar) * 18) / 10) + 32;
  if ((foo % 10) > 4) {            // round up if needed
       ++FWhole;
  }
}

void printTemp(void) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp is: ");
  lcd.setCursor(0,1);   
  
  if (SignBit) {  
     lcd.print("-");
  }
  lcd.print(Whole);
  lcd.print(" C / ");
  lcd.print(FWhole);
  lcd.print(" F");
}

void loop(void) {
  getTemp();
  printTemp();
  delay(1000);
}
int sentDat;

void setup() {
  Serial.begin(9600);   
  pinMode(2, OUTPUT); 
}

void loop() {
  if (Serial.available() > 0) {
	sentDat = Serial.read(); 

	if(sentDat == 'h'){
          //activate the pumpkin for one second and then stop
  	  digitalWrite(2, HIGH);
          delay(1000);
          digitalWrite(2, LOW);
	}
  }
}
