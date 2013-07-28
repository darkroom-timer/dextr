#ifndef Events_h
#define Events_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


class Event {
  public:
  unsigned short type; //event type
  unsigned short data; //additional event data
  
  //methods  
  Event( unsigned short type, unsigned short data);
};


class Events {
  public:
  Event* queue[EVENTS_MAX];
  unsigned short first;
  unsigned short count;
  boolean overflow;
  
  //methods  
  Events( void); //reset all internals
  void add( unsigned short type, unsigned short data); //add to the end and increase count
  Event* get( void); //get first event
  void next( void); //and move pointer to next
};

#endif
