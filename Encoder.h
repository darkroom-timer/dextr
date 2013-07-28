 //////////// ENCODER
#ifndef Encoder_h
#define Encoder_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

extern short int encoder_val;
extern byte waitforpin;
extern short int count;
extern boolean pinAst;
extern boolean pinBst;

void doEncoder( byte pin);
void doEncoderA( void);
void doEncoderB( void);

#endif
