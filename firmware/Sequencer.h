#ifndef Sequencer_h
#define Sequencer_h
/*
 The sequencer holds a number of notes
 Note timing is divided into 24 steps per quarter note
 */

#include "seq.h"

Seq seq;

uint8_t *step_note = seq.step_note;
uint8_t *step_enable = seq.step_enable;

void sequencer_init();
void sequencer_restart();
void sequencer_start();
void sequencer_stop();
void sequencer_advance();
void sequencer_tick_clock();
void sequencer_reset();
void sequencer_update();
void keyboard_to_note();
int keyboard_get_highest_note();
int keyboard_get_latest_note();
void sequencer_align_clock();

uint64_t sequencer_clock = 0;
bool sequencer_is_running = false;

uint32_t next_step_time = 0;
uint32_t gate_off_time = 0;
uint32_t note_on_time;
uint32_t previous_note_on_time;

static bool double_speed = false;

void sequencer_init() {
  for (int i = 0; i < SEQUENCER_NUM_STEPS; i++) {
    step_note[i] = SCALE[random(9)];
  }
  tempo_handler.setHandleTempoEvent(sequencer_tick_clock);
  tempo_handler.setHandleAlignEvent(sequencer_align_clock);
  tempo_handler.setPPQN(PULSES_PER_QUARTER_NOTE);
  sequencer_stop();
  double_speed = false;
  current_step = sequencer.current_step;
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
  sequencer_clock = sequencer.clock;
  sequencer_is_running = sequencer.running;
}

void sequencer_align_clock() {
  // round sequencer_clock to the nearest 12
  if (sequencer_clock % 12 > 6) {
    sequencer_clock += 12 - (sequencer_clock % 12);
  } else {
    sequencer_clock -= (sequencer_clock % 12);
  }
  sequencer.clock = sequencer_clock;
}

void sequencer_reset_clock() { sequencer.clock = sequencer_clock = 0; }

void sequencer_start() {
  MIDI.sendRealTime(midi::Continue);
  usbMIDI.sendRealTime(midi::Continue);
  reset_midi_clock();
  sequencer_is_running = true;
  sequencer.start();
}

void sequencer_stop() {
  if (sequencer.running) {
    usbMIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    MIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    usbMIDI.sendRealTime(midi::Stop);
    MIDI.sendRealTime(midi::Stop);
  }

  sequencer.stop();
  sequencer_is_running = sequencer.running;
  midi_clock = 0;
}

void sequencer_toggle_start() {
  if (sequencer.running) {
    sequencer_stop();
  } else {
    sequencer_start();
  }
}

void sequencer_tick_clock() {
  uint8_t sequencer_divider = PULSES_PER_EIGHT_NOTE;
  if (double_speed) {
    sequencer_divider = PULSES_PER_EIGHT_NOTE / 2;
  }

  if (!tempo_handler.is_clock_source_internal()) {
    int potvalue = synth.speed;
    if (potvalue > 900) {
      sequencer_divider /= 2;
    } else if (potvalue < 127) {
      sequencer_divider *= 2;
    }
  }

  if (sequencer_is_running && (sequencer_clock % sequencer_divider) == 0) {
    sequencer_advance();
  }
  sequencer_clock++;
  sequencer.clock = sequencer_clock;
}

void sequencer_advance() {
  if (!next_step_is_random && !random_flag) {
    random_offset = 0;
  } else {
    random_flag = false;
    random_offset = random(1, (SEQUENCER_NUM_STEPS - 2));
  }

  sequencer.random_offset = random_offset;
  sequencer.advance(millis());
}

void sequencer_update() {
  const int gate_length_msec = map(synth.gateLength, 0, 1023, 10, 200);
  tempo_handler.update(midi_clock);

  sequencer.update(millis(), gate_length_msec);
  sequencer_is_running = sequencer.running;
}

void keyboard_set_note(uint8_t note) { sequencer.activate_note(note); }

void keyboard_unset_note(uint8_t note) { sequencer.deactivate_note(note); }

void keyboard_to_note() { sequencer.keyboard_to_note(millis()); }

#endif
