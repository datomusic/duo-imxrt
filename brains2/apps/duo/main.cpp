#include "Arduino.h"

#include "syscall_stubs.h"
#include "lib/board_init.h"
#include "lib/leds.h"
#include "lib/audio.h"
#include "lib/pin_mux.h"
#include "lib/usb/usb.h"
#include "lib/pins.h"
#include "board_audio_output.h"
#include <Audio.h>
#include "lib/midi_wrapper.h"


#define BENCHMARK(func) digitalWrite(GPIO_SD_13, HIGH); func; digitalWrite(GPIO_SD_13, LOW)

#include "globals.h"

#define SIM_UIDH OCOTP->CFG0
#define SIM_UIDMH OCOTP->CFG1
#define SIM_UIDML 0
#define SIM_UIDL 0

#define MIDI_SYSEX_DATA_TYPE byte
#include "shared/duo/MidiFunctions.h"

RAMFUNCTION_SECTION_CODE(void midi_set_channel(uint8_t channel)) {
  if(channel > 0 && channel <= 16) {
    MIDI_CHANNEL = channel;
  }
}

RAMFUNCTION_SECTION_CODE(uint8_t midi_get_channel()) {
  return MIDI_CHANNEL;
}

#include "lib/sync.h"
#include "lib/elapsedMillis.h"
#include "shared/duo/TempoHandler.h"
TempoHandler tempo_handler;

#include "buttons.h"
#include "shared/duo/Sequencer.h"
#include "duo-firmware/src/Synth.h"

static void midi_handle_clock() {
  tempo_handler.midi_clock_received();
}

static void midi_init() {
  MIDI::init(
      MIDI::Callbacks{.note_on = midi_note_on,
                         .note_off = midi_note_off,
                         .clock = midi_handle_clock,
                         .start = sequencer_start_from_MIDI,
                         .cont = sequencer_start_from_MIDI,
                         .stop = sequencer_stop,
                         .cc = midi_handle_cc,
                         .sysex = midi_handle_sysex});
}

// One more LED than the physical number of leds for loopback testing
static const int NUM_LEDS = 19 + 1;
static const int led_order[NUM_LEDS] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

#define GRB 1
#define SK6812 1
#include "duo-firmware/src/Leds.h"
#include "shared/duo/Pitch.h"

#include "duo-firmware/src/DrumSynth.h"
#include "drums.h"

static uint8_t note_is_playing = 0;
static BoardAudioOutput dac;

void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {
  // Override velocity if button on the synth is pressed
  if (synth.accent) {
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

    MIDI::sendNoteOn(midi_note, velocity, MIDI_CHANNEL);
    envelope1.noteOn();
    envelope2.noteOn();
  } else {
    leds(sequencer.cur_step_index() % Sequencer::NUM_STEPS) = LED_WHITE;
  }
}

void note_off() {
  if (note_is_playing) {
    MIDI::sendNoteOff(note_is_playing, 0, MIDI_CHANNEL);
    envelope1.noteOff();
    envelope2.noteOff();
    note_is_playing = 0;
  }
}


static void pots_read() {
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
  synth.accent = pinRead(ACCENT_PIN);
  synth.delay = pinRead(DELAY_PIN);
}

static void power_off() {
  // Stop sequencer
  MIDI::sendControlChange(123,0,MIDI_CHANNEL);
  AudioNoInterrupts();
  // fade out audio
  // fade out leds
  // turn off audio
  Audio::amp_disable();
  Audio::headphone_disable();

  // turn off leds
  dac.stop();
  // de-enumerate usb (?)
  DatoUSB::disconnect(); 
  led_deinit();
  power_flag = false;
  
  /* pinMode(row_pins[powerbutton_row],INPUT_PULLUP); */
  /* pinMode(col_pins[powerbutton_col],OUTPUT); */
  /* digitalWrite(col_pins[powerbutton_col],LOW); */
  while(keys_scan_powerbutton()) {
  /*   // wait for release of the power button */
  }
  delay(100);
}

bool is_power_on() { return power_flag; }

static void process_key(const char k, const char state) {
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
        const uint8_t step = k - STEP_1;
        if (!sequencer.toggle_step(step)) {
          leds(step) = CRGB::Black;
        }
      } else if (k == BTN_SEQ2) {
        if (!sequencer.is_running()) {
          sequencer.advance();
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
        if (sequencer.is_running()) {
          random_flag = true;
        } else {
          sequencer_randomize_step_offset(sequencer);
        }
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

static void keys_scan() {
  AudioNoInterrupts();
  if (pinRead(DELAY_PIN) && synth.delay == true) {
    synth.delay = false;
    delay_fader.fadeOut(3*440);
    delay_envelope.noteOff();
    mixer_delay.gain(3, 0.0f);
  } else if(!pinRead(DELAY_PIN) && synth.delay == false) {
    synth.delay = true;
    delay_fader.fadeIn(10);
    delay_envelope.noteOn();
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

static void headphone_jack_check() {
  static unsigned long next_jack_check_time = 0;
  const unsigned int jack_check_interval = 200;
  
  if (millis() > next_jack_check_time) {
    next_jack_check_time = millis() + jack_check_interval; 
    if(headphone_jack_detected()) {
    MAIN_GAIN = HEADPHONE_MAIN_GAIN;
    DELAY_GAIN = HEADPHONE_DELAY_GAIN;
    KICK_GAIN = HEADPHONE_KICK_GAIN;
    HAT_GAIN = HEADPHONE_HAT_GAIN;
    Audio::headphone_enable();
    Audio::amp_disable();
  } else {
    MAIN_GAIN = SPEAKER_MAIN_GAIN;
    DELAY_GAIN = SPEAKER_DELAY_GAIN;
    KICK_GAIN = SPEAKER_KICK_GAIN;
    HAT_GAIN = SPEAKER_HAT_GAIN;
    Audio::headphone_disable();
    Audio::amp_enable();
  }
  }
}

static void main_loop(){
  static unsigned long frame_time = millis();
  static unsigned long frame_interval = 11;

  while (true) {
    if (is_power_on()) {
      if (millis() - frame_time > frame_interval) {
        frame_time = millis();
        led_update();
        midi_send_cc();
        FastLED.show();
      } else {
        DatoUSB::background_update();
        MIDI::read(MIDI_CHANNEL);
        pitch_update(); // ~30us
        synth_update(); // ~ 100us
        Drums::update(); // ~ 700us
        sequencer_update();
        headphone_jack_check();
        pots_read();
        keys_scan(); // 14 or 175us (depending on debounce)
      }
    } else {
      if (millis() - frame_time > frame_interval) {
        frame_time = millis();
        if(keys_scan_powerbutton()) { 
          NVIC_SystemReset();
        }
      } 
    }
  }
}

static void main_init(AudioAmplifier& headphone_preamp, AudioAmplifier& speaker_preamp){
  // Read the MIDI channel from EEPROM. Lowest four bits
  // const uint8_t stored_midi_channel =
  //     eeprom_read_byte(EEPROM_MIDI_CHANNEL) & 0xf00;
  const uint8_t stored_midi_channel = 1;
  // if (midi_get_channel() != stored_midi_channel) {
  //   eeprom_write_byte(EEPROM_MIDI_CHANNEL, midi_get_channel());
  // }
  midi_set_channel(stored_midi_channel);

  const uint64_t previous_frame_time = millis();
  // The order sequencer_init, button_matrix_init, led_init and midi_init is
  // important Hold a button of the keyboard at startup to select MIDI channel
  sequencer_init();
  button_matrix_init();
  while(millis() - previous_frame_time < 100) {
    keys_scan();
    /* DatoUSB::background_update(); */
  }
  midi_init();
  led_init();

  dac.begin();

  AudioNoInterrupts();
  headphone_preamp.gain(HEADPHONE_GAIN);
  speaker_preamp.gain(SPEAKER_GAIN);
  audio_init();
  AudioInterrupts();

  Audio::headphone_enable();
  Audio::amp_enable();

  in_setup = false;
}

void init_dma() {
  DMAMUX_Init(DMAMUX);

  edma_config_t userConfig;
  EDMA_GetDefaultConfig(&userConfig);
  EDMA_Init(DMA0, &userConfig);
}

int main(void) {
  board_init();
  init_dma();

  Sync::init();
  LEDs::init();
  pins_init();
  Drums::init();
  delay(USB_STARTUP_DELAY_MS);
  DatoUSB::init();

  //This is needed to configure the UART peripheral correctly (used for MIDI).
  Serial.begin(31250U);

  Audio::amp_disable();
  Audio::headphone_disable();

  AudioAmplifier headphone_preamp;
  AudioAmplifier speaker_preamp;
  AudioConnection patchCord16(pop_suppressor, 0, headphone_preamp, 0);
  AudioConnection patchCord17(pop_suppressor, 0, speaker_preamp, 0);
  AudioConnection patchCord18(headphone_preamp, 0, dac, 0);
  AudioConnection patchCord19(speaker_preamp, 0, dac, 1);

  main_init(headphone_preamp, speaker_preamp);
  main_loop();
  return 0;
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
