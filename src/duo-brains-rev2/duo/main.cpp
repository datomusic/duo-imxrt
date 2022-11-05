#include "Arduino.h"
#include "pinmap.h"
#include "lib/board_init.h"
#include "lib/usb/usb.h"
#include "lib/pin_mux.h"
#include "lib/leds.h"
#include "pinmap.h"
#include <USB-MIDI.h>

typedef int elapsedMillis;
#include "globals.h"

USBMIDI_CREATE_INSTANCE(0, usbMIDI)
#include "stubs/MidiFunctions_stubs.h"
#include "duo-firmware/src/MidiFunctions.h"

#include "duo-firmware/src/TempoHandler.h"
TempoHandler tempo_handler;

#include "buttons.h"

#include "duo-firmware/src/Sequencer.h"
#include "stubs/leds_stub.h"
#include "duo-firmware/src/Leds.h"

#include "amp_controls.h"



int eeprom_read_byte(int _){
  // TODO
  return 0;
}


void note_on(uint8_t midi_note, uint8_t velocity, bool enabled);
void note_off();

int main(void) {
  board_init();
  DatoUSB::init();
  LEDs::init();

  amp_disable();
  headphone_disable();
  sequencer_init();
  audio_init();

  
  // Read the MIDI channel from EEPROM. Lowest four bits
  uint8_t stored_midi_channel = eeprom_read_byte(EEPROM_MIDI_CHANNEL) & 0xf00;
  midi_set_channel(stored_midi_channel);

  // The order sequencer_init, button_matrix_init, led_init and midi_init is important
  // Hold a button of the keyboard at startup to select MIDI channel
  button_matrix_init();
  keys_scan();
  midi_init();
  led_init();

  in_setup = false;

  while (true) {
    loop();
  }
}

void loop(){
    keys_scan();
    FastLED.show();
    delayMicroseconds(40000);
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
