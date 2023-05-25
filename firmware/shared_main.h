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
