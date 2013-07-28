#ifndef events_ids
#define events_ids

enum events_ids {
  EVENT_DUMMY,
  EVENT_BUTTON_DOWN,
  EVENT_BUTTON_LONG,
  EVENT_BUTTON_UP,
  EVENT_ENCODER_MOVE,
  EVENT_REFRESH_DISPLAY,
  EVENT_REFRESH_CONTROLS,
  _EVENT_ACTION_FIRST,
  EVENT_ACTION_STARTSTOP,
  EVENT_ACTION_DECREASE,
  EVENT_ACTION_INCREASE,
  EVENT_ACTION_DECREASEMORE,
  EVENT_ACTION_INCREASEMORE,
  EVENT_ACTION_OK,
  EVENT_ACTION_BACK,
  EVENT_ACTION_MODE,
  EVENT_ACTION_NEXTVAL,
  EVENT_ACTION_LIGHTTOGGLE,
  _EVENT_ACTION_LAST,
  
  //dummy
  DUMMY_LAST_LIST_ITEM2 = 0
};

enum software_modes {
  MODE_DUMMY,
  MODE_MODESELECT,
  MODE_TIMERSIMPLE,
  MODE_TESTSTRIPS,
  MODE_SETTINGS,
  MODE_LIGHTSENSOR,
};

#endif