#include "Arduino.h"

#include "lib/board_init.h"
#include "lib/leds.h"
#include "lib/audio.h"
#include "lib/pin_mux.h"
#include "lib/usb/usb.h"
#include "pins.h"
#include "stubs/arduino_stubs.h"
#include "board_audio_output.h"
#include <Audio.h>
#include <USB-MIDI.h>

unsigned long next_frame_time;
unsigned int frame_interval = 10;

#define BENCHMARK(func) digitalWrite(GPIO_SD_13, HIGH); func; digitalWrite(GPIO_SD_13, LOW)

#include "globals.h"

USBMIDI_CREATE_INSTANCE(0, usbMIDI)

#define setHandleSysEx setHandleSystemExclusive
#define SIM_UIDH OCOTP->CFG0
#define SIM_UIDMH OCOTP->CFG1
#define SIM_UIDML 0
#define SIM_UIDL 0

#include "duo-firmware/src/MidiFunctions.h"

#include "lib/sync.h"
#include "duo-firmware/src/TempoHandler.h"
TempoHandler tempo_handler;

#include "buttons.h"
#include "duo-firmware/src/Sequencer.h"
#include "duo-firmware/src/Synth.h"

// One more LED than the physical number of leds for loopback testing
const int NUM_LEDS = 19 + 1;
const int led_order[NUM_LEDS] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

#define GRB 1
#define SK6812 1
#include "duo-firmware/src/Leds.h"
#include "duo-firmware/src/TouchSlider.h"
#define TOUCH1 16
#define TOUCH2 17
#define TOUCH3 19
#define TOUCH4 18
#include "duo-firmware/src/Pitch.h"
#include "stubs/power_stubs.h"

#include "duo-firmware/src/DrumSynth.h"
#include "drums.h"

void headphone_jack_check();

void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {
  // Override velocity if button on the synth is pressed
  if (pinRead(ACCENT_PIN)) {
    velocity = 127;
  }

  note_is_playing = midi_note;

  if (enabled) {
    AudioNoInterrupts();

    dc1.amplitude(velocity / 127.); // DC amplitude controls filter env amount.
    osc_pulse_midi_note = midi_note;
    osc_pulse_target_frequency = (int)midi_note_to_frequency(midi_note);
    // Detune OSC2
    osc_saw.frequency(detune(osc_pulse_midi_note, detune_amount));

    AudioInterrupts();

    MIDI.sendNoteOn(midi_note, velocity, MIDI_CHANNEL);
    usbMIDI.sendNoteOn(midi_note, velocity, MIDI_CHANNEL);
    envelope1.noteOn();
    envelope2.noteOn();
  } else {
    leds((current_step + random_offset) % SEQUENCER_NUM_STEPS) = LED_WHITE;
  }
}

void note_off() {
  if (note_is_playing) {
    MIDI.sendNoteOff(note_is_playing, 0, MIDI_CHANNEL);
    usbMIDI.sendNoteOff(note_is_playing, 0, MIDI_CHANNEL);
    envelope1.noteOff();
    envelope2.noteOff();
    note_is_playing = 0;
  }
}

void midi_handle_clock() {
  tempo_handler.midi_clock_received();
  midi_clock++;
}

void pots_read() {
  synth.speed = potRead(TEMPO_POT);
  synth.gateLength = potRead(GATE_POT);
  
  synth.resonance = potRead(FILTER_RES_POT);
  synth.release = potRead(AMP_ENV_POT);
  synth.amplitude = potRead(AMP_POT);
  synth.filter = potRead(FILTER_FREQ_POT);
  synth.detune = potRead(OSC_DETUNE_POT);
  synth.pulseWidth = potRead(OSC_PW_POT);

  synth.glide = pinRead(GLIDE_PIN);
  synth.crush = pinRead(BITC_PIN);
}

bool power_check() { return true; }


int main(void) {
  board_init();

  Sync::init();
  LEDs::init();
  pins_init();
  Drums::init();
  DatoUSB::init();

  //This is needed to configure the UART peripheral correctly (used for MIDI).
  Serial.begin(31250U);

  Audio::amp_disable();
  Audio::headphone_disable();
  sequencer_init();

  BoardAudioOutput dac1; // xy=988.1000061035156,100
  AudioAmplifier headphone_preamp;
  AudioAmplifier speaker_preamp;
  AudioConnection patchCord16(pop_suppressor, 0, headphone_preamp, 0);
  AudioConnection patchCord17(pop_suppressor, 0, speaker_preamp, 0);
  AudioConnection patchCord18(headphone_preamp, 0, dac1, 0);
  AudioConnection patchCord19(speaker_preamp, 0, dac1, 1);

  // Read the MIDI channel from EEPROM. Lowest four bits
  // const uint8_t stored_midi_channel =
  //     eeprom_read_byte(EEPROM_MIDI_CHANNEL) & 0xf00;
  const uint8_t stored_midi_channel = 1;
  // if (midi_get_channel() != stored_midi_channel) {
  //   eeprom_write_byte(EEPROM_MIDI_CHANNEL, midi_get_channel());
  // }
  midi_set_channel(stored_midi_channel);

  // The order sequencer_init, button_matrix_init, led_init and midi_init is
  // important Hold a button of the keyboard at startup to select MIDI channel
  next_frame_time = millis() + 100;
  button_matrix_init();
  while(millis() < next_frame_time) {
    keys_scan();
    DatoUSB::background_update();
  }
  next_frame_time = millis() + frame_interval;
  midi_init();
  led_init();

  AudioNoInterrupts();
  headphone_preamp.gain(HEADPHONE_GAIN);
  speaker_preamp.gain(SPEAKER_GAIN);
  audio_init();
  AudioInterrupts();

  MIDI.setHandleStart(sequencer_restart);
  usbMIDI.setHandleStart(sequencer_restart);
  MIDI.setHandleContinue(sequencer_restart);
  usbMIDI.setHandleContinue(sequencer_restart);
  MIDI.setHandleStop(sequencer_stop);
  usbMIDI.setHandleStop(sequencer_stop);

  previous_note_on_time = millis();

  Audio::headphone_enable();
  Audio::amp_enable();

  in_setup = false;  
  
  bool pinState = LOW;
  while (true) {
    digitalWrite(GPIO_SD_13, pinState);
    pinState = !pinState;

    DatoUSB::background_update();

    if (power_check()) {
      midi_handle();
      sequencer_update();

      keyboard_to_note();
      pitch_update(); // ~30us

      synth_update(); // ~ 100us
      midi_send_cc();

      Drums::update(); // ~ 700us

      midi_handle();
      sequencer_update();
      
      headphone_jack_check();
      #ifdef DEV_MODE
      if (!dfu_flag) {
      #endif
      if (millis() > next_frame_time) {
        next_frame_time = millis() + frame_interval;
        led_update();
      } else {
        sequencer_update();
        pots_read();   
        keys_scan(); // 14 or 175us (depending on debounce)
      }
      #ifdef DEV_MODE
      }
      #endif
    }
  }
}

void process_key(const char k, const char state) {
  switch (state) { // Report active key state : IDLE,
                   // PRESSED, HOLD, or RELEASED
    case PRESSED:
      if (k <= KEYB_9 && k >= KEYB_0) {
        if (in_setup) {
          midi_set_channel((k - KEYB_0) + 1);
        } else {
          keyboard_set_note(SCALE[k - KEYB_0]);
        }
      } else if (k <= STEP_8 && k >= STEP_1) {
        step_enable[k - STEP_1] = 1 - step_enable[k - STEP_1];
        if (!step_enable[k - STEP_1]) {
          leds(k - STEP_1) = CRGB::Black;
        }
        step_velocity[k - STEP_1] = INITIAL_VELOCITY;
      } else if (k == BTN_SEQ2) {
        if (!sequencer_is_running) {
          sequencer_advance();
        }
        double_speed = true;
      } else if (k == BTN_DOWN) {
        transpose--;
        if (transpose < -12) {
          transpose = -24;
        }
      } else if (k == BTN_UP) {
        transpose++;
        if (transpose > 12) {
          transpose = 24;
        }
      } else if (k == BTN_SEQ1) {
        next_step_is_random = true;
        if (!sequencer_is_running) {
          sequencer_advance();
        }
        random_flag = true;
      } else if (k == SEQ_START) {
        sequencer_toggle_start();
      }
      break;
    case HOLD:
      if (k <= KEYB_9 && k >= KEYB_0) {
        if (in_setup) {
          midi_set_channel((k - KEYB_0) + 1);
        }
      } else if (k == SEQ_START) {
#ifdef DEV_MODE
        sequencer_stop();
        FastLED.clear();
        FastLED.show();
        delay(1);
        physical_leds[0] = CRGB::Blue;
        FastLED.show();
        dfu_flag = 1;
        BOARD_EnterROMBootloader();
#else
        power_off();
#endif
      }
      break;
    case RELEASED:
      if (k <= KEYB_9 && k >= KEYB_0) {
        keyboard_unset_note(SCALE[k - KEYB_0]);
      } else if (k == BTN_SEQ2) {
        double_speed = false;
      } else if (k == BTN_DOWN) {
        if (transpose < -12) {
          transpose = -12;
        }
        if (transpose > 12) {
          transpose = 12;
        }
      } else if (k == BTN_UP) {
        if (transpose < -12) {
          transpose = -12;
        }
        if (transpose > 12) {
          transpose = 12;
        }
      } else if (k == BTN_SEQ1) {
        next_step_is_random = false;
        random_flag = false;
      } else if (k == SEQ_START) {
#ifdef DEV_MODE
        if (dfu_flag == 1) {
          enter_dfu();
        }
#endif
      }
      break;
    case IDLE:
      if (k <= KEYB_9 && k >= KEYB_0) {
        keyboard_unset_note(k - KEYB_0);
      }
      break;
  }
}

void keys_scan() {
  AudioNoInterrupts();
  if (pinRead(DELAY_PIN) && synth.delay == true) {
    synth.delay = false;

    delay_fader.fadeOut(3*440);
    mixer_delay.gain(0, 0.0f); // Delay input
    mixer_delay.gain(3, 0.0f);
  } else if(!pinRead(DELAY_PIN) && synth.delay == false) {
    synth.delay = true;
    delay_fader.fadeIn(10);
    mixer_delay.gain(0, 0.5f); // Delay input
    mixer_delay.gain(3, 0.4f); // Hat delay input
  }
  AudioInterrupts();

  // scan all the keys and then process them
  if (button_matrix.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {
      if (button_matrix.key[i].stateChanged) {
        const char keycode = button_matrix.key[i].kchar;
        const char state = button_matrix.key[i].kstate;
        process_key(keycode, state);
      }
    }
  }
}

void enter_dfu() {
  // Blank all leds and turn the power button blue before rebooting
  FastLED.clear();
  for(int i = 0; i < NUM_LEDS; i++) {
    physical_leds[i] = CRGB::Black;
  }
  physical_leds[0] = CRGB::Teal;
  FastLED.show();
  delay(100);
  FastLED.show();
  delay(100);
  FastLED.show();
  delay(100);
  FastLED.show();
  BOARD_EnterROMBootloader();
}


void headphone_jack_check() {
  static unsigned long next_jack_check_time = 0;
  const unsigned int jack_check_interval = 200;
  
  if (millis() > next_jack_check_time) {
    next_jack_check_time = millis() + jack_check_interval; 
    if(headphone_jack_detected()) {
    Audio::headphone_enable();
    Audio::amp_disable();
  } else {
    Audio::headphone_disable();
    Audio::amp_enable();
  }
  }
}
