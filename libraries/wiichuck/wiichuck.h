 /*
  * wiichuck.h
  * Arduino library to interface with wii Nunchuck
  */

#ifndef wiichuck_included
#define wiichuck_included

// identities for each field provided by the wii nunchuck
enum nunchuckItems { wii_joyX, wii_joyY, wii_accelX, wii_accelY, wii_accelZ,
wii_btnC, wii_btnZ, wii_ItemCount };

// uses pins adjacent to I2C pins as power & ground for Nunchuck
void nunchuckSetPowerpins();

// initialize the I2C interface for the nunchuck
void nunchuckInit();

// Request data from the nunchuck
void nunchuckRequest();

// Receive data back from the nunchuck,
// returns true if read successful, else false
bool nunchuckRead();

// Encode data to format that most wiimote drivers except
char nunchuckDecode (uint8_t x);

// return the value for the given item
int nunchuckGetValue(int item);

#endif
