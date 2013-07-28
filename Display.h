#ifndef Display_h
#define Display_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <LiquidCrystal_I2C.h>

class Display {
  private:
  LiquidCrystal_I2C *lcd_ptr; //lcd io instance pointer
  unsigned short lcd_cols;
  unsigned short lcd_rows;
  unsigned short buff_size;
  unsigned short part_size;
  unsigned short buff_str_len;
  unsigned short buff_offset;
  public:
  char *buffer; //internal buffer to show
  boolean is_ready; //current state
  //methods  
  Display( LiquidCrystal_I2C *lcd_pt, unsigned short part_sz);
  ~Display( void);
  void reset( void);
  void reset_buffer( void);
  void prepare( char *str = NULL);
  void prepare_row( char *str, unsigned short row);
  void show( char *str = NULL);
};

#endif
