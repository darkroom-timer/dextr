#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Preferences.h"
#include "Events_ids.h"
#include "Events.h"


//////////////////// EVENTS CLASS ////////////////////////

Event::Event( unsigned short t, unsigned short d=0) {
  type = t;
  data = d;
}


//reset all internals
Events::Events( void) {
  first = 0;
  count = 0;
  overflow = false;
}

//add to the end and increase count
void Events::add( unsigned short type, unsigned short data=0) {
  if( count==EVENTS_MAX) {
    overflow = true;
    return;
  }
  
  queue[(first + count) % EVENTS_MAX] = new Event( type, data);
  count++;
}

//get first event
Event* Events::get( void) {
  return queue[first];
}

//and move pointer to next
void Events::next( void) {
  if( count) {
    delete queue[first];
    first = (first + 1) % EVENTS_MAX;
    count--;
  }
}
