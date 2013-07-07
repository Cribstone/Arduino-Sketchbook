#include <AndroidAccessory.h>

bool AndroidAccessory::dataBufferIsEmpty() {
  return (numBytesInDataBuff == nextByteInDataBuffOffset);
}

void AndroidAccessory::refillDataBuffer() {
  uint16_t bytesRead = 0;

  numBytesInDataBuff = nextByteInDataBuffOffset = 0;
  
  // TODO: Add is connected check?

  bytesRead = read(dataBuff, sizeof(dataBuff));

  if (bytesRead >= 1) {
    numBytesInDataBuff = bytesRead;
  }
}

int AndroidAccessory::read() {

  if (dataBufferIsEmpty()) {
    refillDataBuffer();
  }

  return dataBufferIsEmpty() ? -1 : dataBuff[nextByteInDataBuffOffset++];
}

uint16_t AndroidAccessory::read(byte *buff, int len) {
  // Note: This variable is used to both specify
  //       the size of the buffer and store the number
  //       bytes received.
  // TODO: Split this out?
  uint16_t bytesReceived = len;

#if 0
  uint16_t r = RcvData(&bytesReceived, buff);
  if (r == 0) {
    Serial.print("R (buff): ");
    for (int i = 0; i < bytesReceived; i++) {
      Serial.print(buff[i], HEX);
      Serial.print(" ");
    } 
    Serial.println();
  }
  return (r == 0) ? bytesReceived : 0;
#else
  return (RcvData(&bytesReceived, buff) == 0) ? bytesReceived : 0;
#endif
}


int AndroidAccessory::peek() {

  if (dataBufferIsEmpty()) {
    refillDataBuffer();
  }

  return dataBufferIsEmpty() ? -1 : dataBuff[nextByteInDataBuffOffset];
}

int AndroidAccessory::available() {

  // Strictly speaking this doesn't meet the "This is only for bytes
  // that have already arrived" definition from
  // <http://arduino.cc/en/Reference/StreamAvailable> but since the
  // data isn't handled by an ISR it's the only way to avoid hanging
  // waiting for `available()` to return true.
  if (dataBufferIsEmpty()) {
    refillDataBuffer();
  }

  return numBytesInDataBuff - nextByteInDataBuffOffset;
}

size_t AndroidAccessory::write(uint8_t c) {
  return write(&c, 1);
}

size_t AndroidAccessory::write(uint8_t *buff, size_t len) {
  // Returns number of characters written if successful

#if 0
  Serial.print("W: ");
  for (int i = 0; i < len; i++) {
    Serial.print(buff[i], HEX);
    Serial.print(" ");
  } 
  Serial.println();
#endif

  delay(20);

  return (SndData(len, buff) == 0) ? len : 0;
}

void AndroidAccessory::flush() {
  /*
    "Waits for the transmission of outgoing [...] data to complete."

    from <http://arduino.cc/en/Serial/Flush>

    We're treating this as a no-op at the moment.
  */
}
