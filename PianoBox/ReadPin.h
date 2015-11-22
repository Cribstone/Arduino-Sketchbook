#ifndef ReadPin_h
#define ReadPin_h

#include "Arduino.h"

class ReadPin
{
  public:
    uint8_t readCapacitivePin(int pinToMeasure);
};
#endif

