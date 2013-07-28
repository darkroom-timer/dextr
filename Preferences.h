#ifndef preferences_h
#define preferences_h

enum preferences {
  
  //inputs
  PIN_ENCODER_A = 2,   // right
  PIN_ENCODER_B = 3,   // left
  PIN_BUTTON_E = 9,
  PIN_BUTTON_L = 8,
  PIN_BUTTON_R = 10,
  PIN_BUTTON_FSW = 12,
  BUTTONS_TOTAL = 4,
  
  //outputs
  PIN_LCD_BKLIGHT = 6,
  PIN_RELAY1 = 4,
  PIN_RELAY2 = 5,
  PIN_BUZZER = 7,
  
  //lcd params
  LCD_ADDR = 0x38,
  LCD_SIZE_COL = 8,
  LCD_SIZE_ROW = 2,
  LCD_PART_SIZE = 4,
  
  //buttons
  BUTTON_LONG_PRESS = 50, // 500 ms at 100 hz = 50 ticks
  BUTTON_LONG_PRESS_MIN = 20,
  BUTTON_LONG_PRESS_MAX = 100,
  
  //refresh lcd, buttons
  REFRESH_FREQ = 100,
  REFRESH_TIMER_8BIT = (16000000/REFRESH_FREQ/1024)-1, //cnt = F/f/n-1    //=155 @100hz
  REFRESH_UI_DIVIDER = 1,
  
  //relay
  RELAY_TIMER_RESOLUTION = 10, // 10 milliseconds @ 100Hz (double of 50Hz main power)
  
  //misc limits
  EVENTS_MAX = 12, // max events in queue
  EV_MIN = -3 * EVENTS_MAX,
  EV_MAX = 13 * EVENTS_MAX,
  DISPLAY_TIME_PRECISION = 10, // 10 ms
  DISPLAY_BUFF_MAX = LCD_SIZE_COL*LCD_SIZE_ROW+1,
  
  //dummy
  DUMMY_LAST_LIST_ITEM1 = 0
};

#endif
