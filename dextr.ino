
//Project «dextr»
//Darkroom exposure timer
//postpersonality.ru
//
//Software version: v0.1
//Supported hardware versions: HV1

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Math.h>
#include <TimerOne.h>
#include "Preferences.h"
#include "Events_ids.h"
#include "Display.h"
#include "Encoder.h"
#include "Button.h"
#include "Events.h"
#include "Relay.h" //uses Timer1
#include <MemoryFree.h>


//GLOBAL VARIABLES
static byte buttons_pins[BUTTONS_TOTAL] = { PIN_BUTTON_E, PIN_BUTTON_L, PIN_BUTTON_R, PIN_BUTTON_FSW};
static byte buttons_actions[BUTTONS_TOTAL] = { EVENT_ACTION_STARTSTOP, EVENT_ACTION_DECREASE, EVENT_ACTION_INCREASE, EVENT_ACTION_STARTSTOP};
static byte buttons_actions_long[BUTTONS_TOTAL] = { EVENT_ACTION_LIGHTTOGGLE, EVENT_ACTION_DECREASEMORE, EVENT_ACTION_INCREASEMORE, EVENT_ACTION_LIGHTTOGGLE};

LiquidCrystal_I2C lcd( LCD_ADDR, LCD_SIZE_COL, LCD_SIZE_ROW);// LCD 8 symbols x 2 lines
Display display( &lcd, LCD_PART_SIZE);
Button* buttons[BUTTONS_TOTAL];
Events events;
//Relay relay( PIN_RELAY);
int exposure_value = 0;
unsigned short int mode = MODE_TIMERSIMPLE; // simple timer default mode
unsigned short int fstop_fraction = 6; // 1/12th stop

//INIT HARDWARE & SOFTWARE
void setup() {
  //HARDWARE SETUP
  pinMode(PIN_LCD_BKLIGHT, OUTPUT);
  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  
  //encoder init
  pinMode(PIN_ENCODER_A, INPUT); 
  pinMode(PIN_ENCODER_B, INPUT); 
  // turn on pullup resistors for input
  digitalWrite(PIN_ENCODER_A, HIGH);
  digitalWrite(PIN_ENCODER_B, HIGH);

  
  attachInterrupt(0, doEncoderA, CHANGE); // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(1, doEncoderB, CHANGE); // encoder pin on interrupt 1 (pin 3)
  
  //buttons init
  for( int i=0; i<BUTTONS_TOTAL; i++) {
    buttons[i] = new Button( buttons_pins[i], i);
  }

  //LCD setup
  lcd.init();
  //lcd.blink();
  analogWrite( PIN_LCD_BKLIGHT, 255); //set brightness
  
  //Relay
  relay.init( PIN_RELAY1);
  relay.set_callback_update( update_relay_in_action_state);
  
  //Refresh interrupt setup http://arduino.cc/en/Tutorial/SecretsOfArduinoPWM
  // http://forum.arduino.cc/index.php/topic,44283.0.html
  cli(); // Disable interrupts while setting registers
  TCNT2 = 0;
  TCCR2A = _BV(WGM21); // set CTC mode on timer2, outputs (pins) disconnected
  TCCR2B = _BV(CS22)|_BV(CS21)|_BV(CS20); // set 1024 devider on timer2
  OCR2A = REFRESH_TIMER_8BIT; // value to get 100hz frequency
  TIMSK2 = 0; // Reset Timer/Counter2 Interrupt Mask Register
  TIMSK2 |= (1 << OCIE2A); // Enable Output Compare Match A Interrupt  
  sei(); // Enable interrupts once registers have been update

  //Serial.begin(115200);
}

//Refresh on Timer 2 Output A
unsigned short int refresh_divider = 0;
ISR(TIMER2_COMPA_vect) {
  events.add( EVENT_REFRESH_CONTROLS, 0);
  refresh_divider++;
  if( refresh_divider==REFRESH_UI_DIVIDER) {
    refresh_divider = 0;
    events.add( EVENT_REFRESH_DISPLAY, 0);
  }
}





unsigned long millis_last = 0;
// MAIN LOOP: PROCESS EVENT QUEUE
void loop() {
  Event* e;
  
  if( !events.count)
    return;
    
  e = events.get();
  
  Serial.print(  e->type);
  Serial.print( " ");
  millis_last = millis();
  
  
  //EVENT DISPATCHER
  switch( e->type) {
    case EVENT_REFRESH_CONTROLS: {
      refresh_buttons();
      refresh_encoder();
      break;
    }
    case EVENT_REFRESH_DISPLAY: {
      if( display.is_ready) { //build display values
        display.reset_buffer();
        //time
        if( !relay.is_inaction) {
          unsigned long time = ev_to_ms();
          display.prepare_row( display_ms_to_str(time, time), 0);
        } else {
          unsigned long time = relay.time_target - relay.time_current;
          display.prepare_row( display_ms_to_str(time, relay.time_target), 0);
        }
        //ev
        display.prepare_row( display_ev_to_str( exposure_value, fstop_fraction), 1);
      }
      display.show();
      //Serial.println( display.buffer);
      break;
    }
    case EVENT_BUTTON_DOWN: {
      events.add( buttons_actions[e->data], 0);
      break;
    }
    case EVENT_BUTTON_LONG: {
      events.add( buttons_actions_long[e->data], buttons[e->data]->duration_prev ? 1 : 0);
      break;
    }
    case EVENT_ENCODER_MOVE: {
      exposure_value += (int)(e->data);
      ev_value_limit( &exposure_value);
      break;
    }
    case EVENT_BUTTON_UP:
    case EVENT_DUMMY:
    default:
      //EVENT_ACTION fired
      if( e->type > _EVENT_ACTION_FIRST && e->type < _EVENT_ACTION_LAST ) {
        switch( mode) {
          case MODE_TIMERSIMPLE: {
            dispatch_event_action_simpletimer( e->type, e->data);
            break;
          }
          case MODE_MODESELECT: {
            break;
          }
          case MODE_TESTSTRIPS: {
            break;
          }
          case MODE_SETTINGS: {
            break;
          }
          case MODE_LIGHTSENSOR: {
            break;
          }
          default:
            break;
        }
      }
      break;
  }
  
  Serial.print( millis()-millis_last);
  Serial.println("");
  
  events.next();
}

void update_relay_in_action_state( void) {
//  events.add( EVENT_UI_REFRESH, 0);
}


void lcd_test( int i, int row) {
  lcd.setCursor(0, row);
  lcd.print( i);
  lcd.print( ' ');
}
void lcd_test( long int i, int row) {
  lcd.setCursor(0, row);
  lcd.print( i);
  lcd.print( ' ');
}
void lcd_test( float f, int row) {
  lcd.setCursor(0, row);
  lcd.print( f);
  lcd.print( ' ');
}
void lcd_test( unsigned long u, int row) {
  lcd.setCursor(0, row);
  lcd.print( u);
  lcd.print( ' ');
}
void lcd_test( char * str, int row) {
  //lcd.clear();
  //lcd.setCursor( LCD_SIZE_COL-strlen( str), row);
  display_padd_left( str, DISPLAY_BUFF_MAX);
  lcd.setCursor( 0, row);
  lcd.print( str);
}

char display_buff[DISPLAY_BUFF_MAX] = "";

char* display_ms_to_str( unsigned long time_current, unsigned long time_target) { //milliseconds
  display_buff[0] = 0;
  if( time_target < 1000) { // less 1 sec
    snprintf( display_buff, DISPLAY_BUFF_MAX, "0.%02u", (unsigned int)(time_current / DISPLAY_TIME_PRECISION));
  } else if( time_target < 10000) { // less 10 sec
    snprintf( display_buff, DISPLAY_BUFF_MAX, "%u.%02u", (unsigned int)(time_current / 1000), (unsigned int)((time_current / DISPLAY_TIME_PRECISION)%100) );
  } else if( time_target < 60000) { // less 1 min
    snprintf( display_buff, DISPLAY_BUFF_MAX, "%02u.%u", (unsigned int)(time_current / 1000), (unsigned int)((time_current / (DISPLAY_TIME_PRECISION*10))%10) );
  } else if( time_target < 3600000) { // less 1 hour
    snprintf( display_buff, DISPLAY_BUFF_MAX, "%02u:%02u", (unsigned int)(time_current / 60000), (unsigned int)((time_current / 1000)%60) );
  } else { // more of equal 1 hour
    snprintf( display_buff, DISPLAY_BUFF_MAX, "%u:%02u.%02u", (unsigned int)(time_current / 3600000), (unsigned int)((time_current / 60000)%60), (unsigned int)((time_current / 1000)%60) );
  }
  //display_buff_fill_up_spaces( display_buff, DISPLAY_BUFF_MAX);
  //display_padd_left( display_buff, DISPLAY_BUFF_MAX);
  return display_buff;
}

char* display_ev_to_str( int ev, int fraction) { //milliseconds
  display_buff[0] = 0;
  snprintf( display_buff, DISPLAY_BUFF_MAX, "%s%u/%02i", ev>=0 ? "" : "-", abs(ev / fraction), (abs(ev) % fraction) );
  //display_padd_left( display_buff, DISPLAY_BUFF_MAX);
  return display_buff;
}

void display_padd_right( char *str, unsigned short int n) {
  unsigned short i = strlen(str);
  if( i >= n)
    return;
  //n--;
  for( ; i < n; i++) {
    str[i] = ' ';
  }
  str[i] = 0;
}

void display_padd_left( char *str, signed short int n) {
  signed short i;
  unsigned short l = strlen(str);
  n--;
  if( l >= n)
    return;
  str[n] = 0;
  n--;
  for( i=0 ; i < l; i++) {
    str[n] = str[l-i-1];
    n--;
  }
  for( ; n >= 0; n--) {
    str[n] = ' ';
  }
}

int ev_to_ms_cache_key = -1;
unsigned long ev_to_ms_cache_val = 0;
unsigned long ev_to_ms( void) {
  if( exposure_value!=ev_to_ms_cache_key) {
    ev_to_ms_cache_key = exposure_value;
    ev_to_ms_cache_val = round(pow( 2.0, (float)exposure_value/(float)fstop_fraction)*1000.0/RELAY_TIMER_RESOLUTION)*RELAY_TIMER_RESOLUTION;
  }
  return ev_to_ms_cache_val;
}

void ev_value_limit( int *ev) {
  if( ev[0] < EV_MIN)
    ev[0] = EV_MIN;
  else if( ev[0] > EV_MAX)
    ev[0] = EV_MAX;
}




/////////////// PROCESS QUEQUE FUNCTIONS

void refresh_buttons( void) {
  byte event;
  for( int i=0; i<BUTTONS_TOTAL; i++) {
    buttons[i]->refresh();
    event = buttons[i]->get_event();
    if( event) {
      events.add( event, i);
    }
  }
}

void refresh_encoder( void) {
  if( encoder_val) {
    events.add( EVENT_ENCODER_MOVE, (unsigned short int) encoder_val);
    encoder_val = 0;
  }
}

void dispatch_event_action_simpletimer( unsigned short type, unsigned short data) {
  //EVENT DISPATCHER
  switch( type) {  
    case EVENT_ACTION_STARTSTOP: {
      if( !relay.is_inaction) {
        unsigned long time = ev_to_ms();
        //lcd_test( time, 0);
        relay.start( time);
      } else {
        relay.stop();
      }
      break;
    }
    case EVENT_ACTION_DECREASE: {
      events.add( EVENT_ENCODER_MOVE, -1);
      break;
    }
    case EVENT_ACTION_INCREASE: {
      events.add( EVENT_ENCODER_MOVE, +1);
      break;
    }
    case EVENT_ACTION_DECREASEMORE: {
      events.add( EVENT_ENCODER_MOVE, -fstop_fraction+(data ? 1 : 0)); //compensate previous EVENT_ACTION_DECREASE
      break;
    }
    case EVENT_ACTION_INCREASEMORE: {
      events.add( EVENT_ENCODER_MOVE, +fstop_fraction-(data ? 1 : 0)); //compensate previous EVENT_ACTION_INCREASE
      break;
    }
    case EVENT_ACTION_OK: {
      break;
    }
    case EVENT_ACTION_BACK: {
      break;
    }
    case EVENT_ACTION_MODE: {
      break;
    }
    case EVENT_ACTION_NEXTVAL: {
      break;
    }
    case EVENT_ACTION_LIGHTTOGGLE: {
      if( data) //first long press
        relay.toggle();
      break;
    }
    default:
      break;
  }
}

