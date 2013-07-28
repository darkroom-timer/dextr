#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <TimerOne.h>
#include "Preferences.h"
#include "Relay.h"

Relay relay;

  //extern 
//////////////////// RELAY CLASS ////////////////////////

  Relay::Relay( void) { //reset state & init hw 
    //reset state
    pin = 0;
    time_target = 0;
    time_current = 0;
    is_inaction = false;
    is_on = false;
    callback_update = relay_empty_function;
    callback_finish = relay_empty_function;
  }
    
  void Relay::init( unsigned short p) { //reset state & init hw 
    pin = p;
    //init hw
    pinMode( pin, OUTPUT);
    Timer1.initialize( RELAY_TIMER_RESOLUTION*1000); // set a timer of length 10000 microseconds (or 0.01 sec - or 100Hz)
    Timer1.disablePwm( pin);
    Timer1.start();
    Timer1.stop();
    Timer1.attachInterrupt( relay_time_count); // attach the service routine here
  }
  
  void Relay::on( void) {
    //is_on = true;
    digitalWrite( pin, HIGH);
  }
  
  void Relay::off( void) {
    //is_on = false;
    digitalWrite( pin, LOW );
  }
  
  void Relay::toggle( void) { //resets startstop
    stop();
    if( is_on) {
      off();
      is_on = false;
    } else {
      on();
      is_on = true;
    }
  }
   
  void Relay::start( unsigned long time) { //resets toggle
    off();
    time_target = time / RELAY_TIMER_RESOLUTION * RELAY_TIMER_RESOLUTION; //round-up
    time_current = 0;
    if( !time_target)
      return;
    is_inaction = true;
    on();
    Timer1.resume();
  }
    
  void Relay::stop( void) {
    Timer1.stop();
    is_inaction = false;
    off();
  }
  
  
  void Relay::set_callback_finish( void (*cb)()) {
    callback_finish = cb;
  }
  
  void Relay::set_callback_update( void (*cb)()) {
    callback_update = cb;
  }
  
  void relay_empty_function( void) {
  }
  
  
  void relay_time_count( void) {
    relay.time_current += RELAY_TIMER_RESOLUTION;
    if( relay.time_current>=relay.time_target) {
      relay.stop();
      //relay.callback_finish();
    } else {
      //relay.callback_update();
    }
  }
  
