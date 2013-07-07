#ifndef __AndroidAccessory_h__
#define __AndroidAccessory_h__

#include "Arduino.h"
#include "adk.h"
#include "Stream.h"

#define DATA_BUFFER_SIZE 64

// TODO: Merge `ADK` & `ArduinoAccessory`?

class AndroidAccessory : private ADK, public Stream {

private:
  byte dataBuff[DATA_BUFFER_SIZE];
  unsigned int numBytesInDataBuff;
  unsigned int nextByteInDataBuffOffset;

  bool dataBufferIsEmpty();
  void refillDataBuffer();
  
  // Private because it bypasses the data buffer.
  uint16_t read(byte *buff, int len);

public:
 AndroidAccessory(const char* manufacturer,
                  const char* model,
                  const char* description = "An Arduino-based Android accessory",
                  const char* version = "1.0",
                  const char* uri = "http://labs.arduino.cc/ADK/Index",
		  const char* serial = "00000000") :
  // TODO: Remove `Usb` from ADK constructor also?
  ADK(UsbHost, manufacturer, model, description, version, uri, serial) {};

  bool isConnected(void) {return isReady();};

  void refresh(void) {pUsb.Task();};

  // TODO: Allow parameters to be supplied?
  boolean begin(void) {return (pUsb.Init() != -1);}; 


  // Implement Stream interface
  virtual size_t write(uint8_t *buff, size_t len);

  virtual int available(void);
  virtual int peek(void);
  virtual int read(void);

  virtual void flush();
  virtual size_t write(uint8_t);

  using Print::write; // pull in write(str) and write(buf, size) from Print

};

#endif
