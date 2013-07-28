#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Preferences.h"
#include "Events_ids.h"
#include "Button.h"

//init
Button::Button( byte p, byte id=255) {
  if( id==255)
    id = p;
  set_pin( p);
  reset();
}
//reset internal variables
void Button::reset( void) {
  duration = 0;
  duration_prev = 0;
  state_prev = false;
  state = false;
  //state_prev = digitalRead( pin);
  //state = digitalRead( pin);
}
//refresh internal variables
void Button::refresh( void) {
  state_prev = state;
  state = !digitalRead( pin);
  if( state==state_prev)
    duration++;
  else {
    duration_prev = duration;
    duration = 0;
  }
}
//get current state
byte Button::get_event( void) {
  if( state) { //press
    if( !state_prev) {
      return EVENT_BUTTON_DOWN;
    }
    if( duration && !(duration % BUTTON_LONG_PRESS) ) {
      if( duration != BUTTON_LONG_PRESS)
        duration_prev = 0;
      //duration = 0;
      return EVENT_BUTTON_LONG;
    }
  } else { //release
    if( state_prev!=state) {
      return EVENT_BUTTON_UP;
    }
  }
  return 0; //no event
}
//set pin, init HW
void Button::set_pin( byte p) {
  pin = p;
  pinMode( pin, INPUT);
  digitalWrite( pin, HIGH);
}
