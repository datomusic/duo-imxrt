#ifndef Sequencer_h
#define Sequencer_h
/*
 The sequencer holds a number of notes
 Note timing is divided into 24 steps per quarter note
 */

uint32_t last_sequencer_update;
uint32_t sequencer_delta_accum = 0;

void sequencer_init();
void sequencer_restart();
void sequencer_start();
void sequencer_stop();
void sequencer_tick_clock();
void sequencer_reset();
void sequencer_update();
void keyboard_to_note();
int keyboard_get_highest_note();
int keyboard_get_latest_note();
void sequencer_align_clock();

static bool double_speed = false;

void sequencer_randomize_step_offset() {
  sequencer.step_offset = random(1, (Sequencer::NUM_STEPS - 2));
  if (!sequencer.is_running()) {
    sequencer.advance();
  }
}

void sequencer_init() {
  last_sequencer_update = millis();
  for (int i = 0; i < Sequencer::NUM_STEPS; i++) {
    sequencer.set_step_note(i, SCALE[random(9)]);
  }

  tempo_handler.setHandleTempoEvent(sequencer_tick_clock);
  tempo_handler.setHandleAlignEvent(sequencer_align_clock);
  tempo_handler.setPPQN(Sequencer::PULSES_PER_QUARTER_NOTE);
  sequencer_stop();
  double_speed = false;
}

static void reset_midi_clock() {
  midi_clock = 0;
  tempo_handler.midi_clock_reset();
}

void sequencer_restart() {
  MIDI.sendRealTime(midi::Start);
  delay(1);
  reset_midi_clock();
  sequencer.restart();
}

void sequencer_align_clock() { sequencer.align_clock(); }

void sequencer_start() {
  MIDI.sendRealTime(midi::Continue);
  usbMIDI.sendRealTime(midi::Continue);
  reset_midi_clock();
  sequencer.start();
}

void sequencer_stop() {
  if (sequencer.is_running()) {
    usbMIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    MIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    usbMIDI.sendRealTime(midi::Stop);
    MIDI.sendRealTime(midi::Stop);
  }

  sequencer.stop();
  midi_clock = 0;
}

void sequencer_toggle_start() {
  if (sequencer.is_running()) {
    sequencer_stop();
  } else {
    sequencer_start();
  }
}

void sequencer_tick_clock() {
  if (!tempo_handler.is_clock_source_internal()) {
    int potvalue = synth.speed;
    if (potvalue > 900) {
      sequencer.speed_mod = Sequencer::DoubleSpeed;
    } else if (potvalue < 127) {
      sequencer.speed_mod = Sequencer::HalfSpeed;
    }
  } else if (double_speed) {
    sequencer.speed_mod = Sequencer::DoubleSpeed;
  } else {
    sequencer.speed_mod = Sequencer::NormalSpeed;
  }

  if (sequencer.tick_clock(sequencer_delta_accum)) {
    if (random_flag) {
      sequencer.step_offset = random(1, (Sequencer::NUM_STEPS - 2));
    } else {
      sequencer.step_offset = 0;
    }
  }

  sequencer_delta_accum = 0;
}

void sequencer_update() {
  sequencer.gate_length_msec = map(synth.gateLength, 0, 1023, 10, 200);
  tempo_handler.update(midi_clock);

  const uint32_t cur_millis = millis();
  sequencer_delta_accum += (cur_millis - last_sequencer_update);
  last_sequencer_update = cur_millis;
}

void keyboard_set_note(uint8_t note) { sequencer.hold_note(note); }

void keyboard_unset_note(uint8_t note) { sequencer.release_note(note); }

#endif
