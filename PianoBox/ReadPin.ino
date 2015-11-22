#include "Arduino.h"
#include "ReadPin.h"

uint8_t ReadPin::readCapacitivePin(int pinToMeasure)
{
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  *ddr &= ~(bitmask);
  *port |= bitmask;
  int cycles = 17;
  for(int i = 0; i < 16; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}
