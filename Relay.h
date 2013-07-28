#ifndef Relay_h
#define Relay_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <TimerOne.h>

class Relay {
  public:
  unsigned short pin;
  unsigned long time_target;
  unsigned long time_current;
  boolean is_inaction;
  boolean is_on;
  void (*callback_finish)();
  void (*callback_update)();
  
  //methods  
  Relay( void); //reset state & init hw 
  void init( unsigned short pin); //reset state & init hw 
  void on( void);
  void off( void);
  void toggle( void); //resets startstop
  void start( unsigned long time);  //resets toggle
  void stop( void);
  void set_callback_finish( void (*cb)());
  void set_callback_update( void (*cb)());

};

  extern Relay relay;
  void relay_empty_function( void);
  void relay_time_count( void);
  
#endif
