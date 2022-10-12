#include "Arduino.h"
#include "pinmap.h"
#include "board_init.h"
#include "pinmap.h"
#include "pin_mux.h"
#include "leds.h"

#include "stubs/sequencer_stub.h"
#include "globals.h"
#include "duo1/Sequencer.h"
void keyboard_event(char key, char state);
void step_event(char key, char state);
void button_play_event(char state);

#include "Buttons.h"

#include "stubs/leds_stub.h"
#include "duo1/Leds.h"



int main(void) {
  board_init();
  LEDs::init();

  FastLED.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(physical_leds, NUM_LEDS);
  
  button_matrix_init();

for (int i = 0; i < NUM_LEDS; ++i) {
    physical_leds[i] = 0;
  }
  
  for(int i = 0; i < 10; i++) {
    physical_leds[i+9] = COLORS[SCALE[i]%24];
    delay(20);
    FastLED.show();
  }

  while (1) {
    keys_scan();
    FastLED.show();
    delayMicroseconds(40000);
  }
}

bool process_key(char keycode, char state)
{
  switch (keycode) {
    case SEQ_START :
      button_play_event(state);
      break;

    case KEYB_0 ... KEYB_9 :
      keyboard_event(keycode - KEYB_0, state);
      break;

    case STEP_1 ... STEP_8 :
      step_event(keycode - STEP_1, state);
      break;

    case BTN_DOWN ... BTN_UP :
      step_event(keycode - BTN_DOWN , state);
      break;
    
    case BTN_SEQ1 ... BTN_SEQ2 :
      step_event(keycode - BTN_SEQ1, state);
      break;

    default:
      break;
  }
  return false;
}

void keyboard_event(char key, char state) {
  const int KEYBOARD_FIRST_LED = 9;

  switch (state) {
    case PRESSED:
       physical_leds[key+KEYBOARD_FIRST_LED] = 0x00FF00;
       break;
    case HOLD:
       physical_leds[key+KEYBOARD_FIRST_LED] = 0xFF0000;
       break;
    case RELEASED:
      physical_leds[key+KEYBOARD_FIRST_LED] = 0x000080;
      break;
  }
}

void step_event(char step, char state) {
  switch (state) {
    case PRESSED:
       physical_leds[step+1] = 0x00FF00;
       return;
    case HOLD:
       physical_leds[step+1] = 0xFF0000;
       return;
    case RELEASED:
      physical_leds[step+1] = 0x000080;
      return;
    default:
      return;
  }
}

void button_play_event(char state) {
   switch (state) {
    case PRESSED:
       physical_leds[0] = 0x00FF00;
       return;
    case HOLD:
       physical_leds[0] = 0xFF0000;
       return;
    case RELEASED:
      physical_leds[0] = 0x000080;
      return;
    default:
      return;
  } 
}