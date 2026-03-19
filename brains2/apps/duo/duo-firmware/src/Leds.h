#ifndef Leds_h
#define Leds_h

#include <FastLED.h>
#include "led_pins.h"

#define COLOR_ORDER GRB

#define LED_TYPE SK6805
#define LED_CORRECTION 0xFFD3E0
#define LED_WHITE CRGB(230,255,150)

#define leds(A) physical_leds[led_order[A]]

CRGB physical_leds[NUM_LEDS];
#define led_play physical_leds[0]

#define MAX_BRIGHTNESS 140

// Maps led_brightness_level (0-9) to FastLED brightness values
static const uint8_t BRIGHTNESS_TABLE[] = {
  14, 28, 42, 56, 70, 84, 98, 112, 126, MAX_BRIGHTNESS
};

static inline uint8_t get_neopixel_brightness() {
  return BRIGHTNESS_TABLE[led_brightness_level];
}

// Analog LED brightness scale factor: 0.1 to 1.0 in 0.1 steps
// Returns value scaled by brightness level
static inline uint16_t scale_analog_led(uint16_t value) {
  return (uint16_t)((uint32_t)value * (led_brightness_level + 1) / 10);
}

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
void led_deinit();
void led_show_brightness_mode();

void led_init() {
  FastLED.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(physical_leds, NUM_LEDS);

  FastLED.setBrightness(get_neopixel_brightness());
  FastLED.setCorrection(LED_CORRECTION);

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

// Shows a bar graph on the step LEDs indicating current brightness level
void led_show_brightness_mode() {
  FastLED.setBrightness(get_neopixel_brightness());

  for (uint16_t i = 0; i < 10; i++) {
    physical_leds[i + 9] = CRGB::Black;
  }
  for (int l = 0; l < Sequencer::NUM_STEPS; l++) {
    if (l <= (int)led_brightness_level) {
      leds(l) = LED_WHITE;
    } else {
      leds(l) = CRGB::Black;
    }
  }
  led_play = CRGB::Black;

  blank_env_led();
  blank_filter_led();
  blank_osc_led();
}

// Updates the LED colour and brightness to match the stored sequence
void led_update() {
  if (brightness_mode) {
    led_show_brightness_mode();
    return;
  }

  FastLED.setBrightness(get_neopixel_brightness());
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

  if (sequencer.gate_active()) {
    leds(cur_seq_step) = LED_WHITE;
  }

  if (sequencer.is_running()) {
    led_play = LED_WHITE;
  } else {
    if (sequencer.note_playing()) {
      leds(Sequencer::wrapped_step(cur_seq_step)) = LED_WHITE;
    } else {
      const unsigned step_ticks = Sequencer::TICKS_PER_STEP;
      const uint32_t seq_clock = sequencer.get_clock() + step_ticks;
      const uint32_t fade_val = (seq_clock % step_ticks) * 16;
      const bool fade_play = (seq_clock % (2 * step_ticks)) < step_ticks;

      // Toggle between fading play button or current step.
      if (fade_play) {
        led_play = LED_WHITE;
        led_play.fadeLightBy(fade_val);
      } else {
        led_play = CRGB::Black;

        if (sequencer.get_step_enabled(cur_seq_step)) {
          leds(cur_seq_step) = blend(LED_WHITE, COLORS[sequencer.get_step_note(cur_seq_step) % 24], fade_val);
        } else {
          leds(cur_seq_step) = LED_WHITE;
          leds(cur_seq_step) = leds(cur_seq_step).fadeLightBy(fade_val);
        }
      }
    }
  }

  write_env_led(scale_analog_led(peak1.read()));
  write_filter_led(scale_analog_led(synth.filter));
  write_osc_led(scale_analog_led(synth.pulseWidth));
}

#endif
