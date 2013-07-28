#ifndef Button_h
#define Button_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Button {
  public:
  unsigned long duration; //time from previous state change
  unsigned long duration_prev; //time before previous state change
  boolean state_prev; //previous state
  boolean state; //current state
  byte pin; //arduino pin number
  byte id; //arduino pin number
  //methods  
  Button( byte, byte);
  void refresh( void);
  void reset( void);
  byte get_event( void);
  void set_pin( byte);
};

#endif
