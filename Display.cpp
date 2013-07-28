#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <LiquidCrystal_I2C.h>
#include "Preferences.h"
#include "Events_ids.h"
#include "Display.h"

//init  
Display::Display( LiquidCrystal_I2C *lcd_pt, unsigned short part_sz) {
  lcd_ptr = lcd_pt;
  lcd_cols = lcd_ptr->get_cols();
  lcd_rows = lcd_ptr->get_rows();
  buff_size = lcd_rows*lcd_cols;
  part_size = part_sz;
  buffer = new char[buff_size+1];
  reset();
}

//deinit
Display::~Display( void) {
  delete[] buffer;
}

//reset internal variables
void Display::reset( void) {
  void reset_buffer( void);
  buff_offset = 0;
  is_ready = true;
}

//fill buffer with spaces
void Display::reset_buffer( void) {
  memset( buffer, ' ', buff_size);
  buffer[buff_size] = 0;
}

void Display::prepare( char *str) {
  if( str == NULL) {
    str = buffer;
  }
  buff_str_len = strlen( str);
  buff_str_len = buff_str_len > buff_size ? buff_size : buff_str_len; //min( len, buff_size)
  if( str != buffer) {
    reset_buffer();
    memcpy( buffer, str, buff_str_len);
    buffer[buff_str_len] = 0;
  }
  is_ready = false;
  buff_offset = 0;
}

void Display::prepare_row( char *str, unsigned short row) {
  unsigned short len = strlen( str);
  len = len>lcd_cols ? lcd_cols : len;
  //memset( buffer + row * lcd_cols, ' ', lcd_cols);
  memcpy( buffer + ((row+1) * lcd_cols - len), str, len);
  //buffer[buff_size] = 0;
}

void Display::show( char *str) {
  if( is_ready) { //ready to load buffer
    prepare( str);
  }
  
  //show next part
  buff_offset += part_size;
  char tmp = buffer[buff_offset];
  buffer[buff_offset] = 0;
  buff_offset -= part_size;
  lcd_ptr->setCursor( buff_offset % lcd_cols, buff_offset / lcd_cols);
  lcd_ptr->print( buffer + buff_offset);
  buff_offset += part_size;
  buffer[buff_offset] = tmp;
  
  if( buff_offset >= buff_str_len) { //check completeness of show-task
    is_ready = true;
  }
}

