 //////////// ENCODER


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Preferences.h"
#include "Events_ids.h"
#include "Encoder.h"

short int encoder_val = 0;
byte waitforpin = 2;
short int count = 0;
boolean pinAst;
boolean pinBst;

void doEncoder( byte pin){
  //!!! HARD PROGRAMMED PORT/PINS !!! BEWARE 
  boolean pinA = (PIND & (1 << PIN_ENCODER_A)) >> PIN_ENCODER_A;
  boolean pinB = (PIND & (1 << PIN_ENCODER_B)) >> PIN_ENCODER_B;
  short int dir = 0;
  boolean pinV;
    pinV = !pinBst;
    pinV = (pinV^pinAst)|pinV;
    pinV = !pinV;
  
  if( waitforpin==2) {
    pinAst = pin ? !pinA : pinA;
    pinAst = !pin ? !pinB : pinB;
  }
  
  if( pin) { //A
    if( pinA!=pinAst) {
      pinAst = pinA;
      waitforpin = !pin;
      if( pinA==pinBst)
        dir--;
       else
        dir++;
    }
  } else { //B
    if( pinB!=pinBst) {
      pinBst = pinB;
      waitforpin = !pin;
      if( pinB==pinAst)
        dir++;
       else
        dir--;
    }
  }
  count += dir;
  if( count==4) {
    count=0;
    encoder_val--;
  }
  if( count==-4) {
    count=0;
    encoder_val++;
  }
}

// Interrupt on A changing state
void doEncoderA( void){
  doEncoder( 0);
}
void doEncoderB( void){
  doEncoder( 1);
}


