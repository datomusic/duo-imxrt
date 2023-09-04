#ifndef Leds_h
#define Leds_h

#include <FastLED.h>
#include "led_pins.h"

#define COLOR_ORDER GRB

#define LED_TYPE SK6812
#define CORRECTION_SK6812 0xFFF1E0
#define CORRECTION_SK6805 0xFFD3E0
#define LED_WHITE CRGB(230,255,150)

#define leds(A) physical_leds[led_order[A]]

CRGB physical_leds[NUM_LEDS];
#define led_play physical_leds[0]

#define SK6812_BRIGHTNESS 32
#define SK6805_BRIGHTNESS 140

/* The black keys have assigned colors. The white keys are shown in gray */
const CRGB COLORS[] = {
  0x444444,
  0xFF0001,
  0x444444,
  0xFFDD00,
  0x444444,
  0x444444,
  0x11FF00,
  0x444444,
  0x0033DD,
  0x444444,
  0xFF00FF,
  0x444444,
  0x444444,
  0xFF2209,
  0x444444,
  0x99FF00,
  0x444444,
  0x444444,
  0x00EE22,
  0x444444,
  0x0099CC,
  0x444444,
  0xBB33BB,
  0x444444
};

void led_init();
void led_update();
void led_data_received();
void led_deinit();

void led_data_received() {
    FastLED.setBrightness(SK6805_BRIGHTNESS); 
    FastLED.setCorrection(CORRECTION_SK6805);
    detachInterrupt(LED_CLK);
}

void led_init() {
  FastLED.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(physical_leds, NUM_LEDS);
  
  FastLED.setBrightness(SK6805_BRIGHTNESS); 
  FastLED.setCorrection(CORRECTION_SK6812);

  // We're going to do a loopback test first to determine brightness
  //attachInterrupt(LED_CLK, led_data_received, CHANGE);
  FastLED.clear();
  physical_leds[NUM_LEDS-1] = CRGB(0xff6805);
  FastLED.show();
  
  FastLED.clear();
  FastLED.show();
  /* The 400ms delay introduced by this startup animation prevents
     an audible pop/click at startup
     */
     
  #ifdef DEV_MODE
     physical_leds[0] = CRGB::Blue;
  #endif

  // The key of the current MIDI channel lights up
  physical_leds[MIDI_CHANNEL+8] = COLORS[SCALE[MIDI_CHANNEL-1]%24];

  FastLED.show();
  delay(100);
  FastLED.show();
  delay(100);
  FastLED.show();
  delay(100);

  for(uint16_t i = 0; i < 10; i++) {
    write_env_led(i*8);
    write_filter_led(i*8);
    write_osc_led(i*8);

    physical_leds[i+9] = COLORS[SCALE[i]%24];
    delay(20);
    FastLED.show();
  }
}

// Fades out leds and sets them to a low power state if possible
void led_deinit() {
  for(int i = 32; i > 1; i = (i*7)>>3) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(20);
  }

  blank_env_led();
  blank_filter_led();
  blank_osc_led();
  FastLED.clear();
  FastLED.show();
}

// Updates the LED colour and brightness to match the stored sequence
void led_update() {
  for (uint16_t i = 0; i < 10; i++) {
    physical_leds[i + 9] = COLORS[SCALE[i] % 24];
  }

  for (int l = 0; l < Sequencer::NUM_STEPS; l++) {
    if (sequencer.get_step_enabled(l)) {
      leds(l) = COLORS[sequencer.get_step_note(l) % 24];
    } else {
      leds(l) = CRGB::Black;
    }
  }

  const auto cur_seq_step = sequencer.cur_step_index();
  const bool cur_step_enabled = sequencer.get_step_enabled(cur_seq_step);
  const auto seq_clock = sequencer.get_clock();

  if (!sequencer.is_running()) {
    if (sequencer.note_playing()) {
      leds(Sequencer::wrapped_step(cur_seq_step - 1)) = LED_WHITE;
    } else {
      if (((seq_clock % 24) < 12)) {
        if (cur_step_enabled) {
          leds(cur_seq_step) =
              COLORS[sequencer.get_step_note(cur_seq_step) % 24];
        } else {
          leds(cur_seq_step) = CRGB::Black;
        }
        led_play = LED_WHITE;
        led_play.fadeLightBy((seq_clock % 12) * 16);
      } else {
        led_play = CRGB::Black;
        if (cur_step_enabled) {
          leds(cur_seq_step) = blend(
              LED_WHITE, COLORS[sequencer.get_step_note(cur_seq_step) % 24],
              (seq_clock % 12) * 16);
        } else {
          leds(cur_seq_step) = LED_WHITE;
          leds(cur_seq_step) =
              leds(cur_seq_step).fadeLightBy((seq_clock % 12) * 16);
        }
      }
    }
  } else {
    led_play = LED_WHITE;
  }

  write_env_led(peak1.read());
  write_filter_led(synth.filter);
  write_osc_led(synth.pulseWidth);
}

#endif
