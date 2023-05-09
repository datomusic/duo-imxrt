#ifndef Sequencer_h
#define Sequencer_h
/*
 The sequencer holds a number of notes
 Note timing is divided into 24 steps per quarter note
 */

#include "firmware/note_stack.h"
#include <cstdint>

#define PULSES_PER_EIGHT_NOTE PULSES_PER_QUARTER_NOTE / 2
const uint8_t SEQUENCER_NUM_STEPS = 8;

// Initial sequencer values

uint32_t previous_note_on_time;
static bool double_speed = false;
uint32_t sequencer_clock = 0;
uint8_t current_step = 0;
uint8_t note_is_playing = 0;
int random_offset = 0;

namespace Sequencer {
static const uint8_t INITIAL_VELOCITY = 100;
uint8_t step_enable[SEQUENCER_NUM_STEPS] = {1, 0, 1, 1, 1, 1, 0, 1};
uint8_t step_note[SEQUENCER_NUM_STEPS] = {1, 0, 6, 9, 0, 4, 0, 5};
uint8_t step_velocity[SEQUENCER_NUM_STEPS] = {100, 100, 100, 100,
                                              100, 100, 100, 100};

static bool note_is_done_playing = false;
static uint32_t note_off_time = 0;
static int gate_length_msec = 40;
static bool random_flag = 0;
static bool next_step_is_random = false;

NoteStack<10> note_stack;

void align_clock() {
  tempo_handler.align_clock();
}

void sequencer_trigger_note() {
  note_is_triggered = true;
  note_is_done_playing = false;
  previous_note_on_time = millis();
  note_off_time = previous_note_on_time + gate_length_msec;

  step_velocity[current_step] = INITIAL_VELOCITY;
  note_is_done_playing = true;
  note_off();
  note_is_triggered = false;
  note_off_time =
      millis() + tempo_interval -
      gate_length_msec; // Set note off time to sometime in the future
}

static uint8_t arpeggio_index = 0;
void sequencer_advance_without_play() {

  if (!note_is_done_playing) {
    sequencer_untrigger_note();
  }
  current_step++;
  current_step %= SEQUENCER_NUM_STEPS;

  if (!next_step_is_random && !random_flag) {
    random_offset = 0;
  } else {
    random_flag = false;
    random_offset = random(1, (SEQUENCER_NUM_STEPS - 2));
  }

  // Sample keys
  uint8_t n = note_stack.size();

  if (arpeggio_index >= n) {
    arpeggio_index = 0;
  }

  if (n > 0) {
    if (!running) {
      step_note[current_step] = note_stack.most_recent_note().note;
    } else {
      step_note[current_step] = note_stack.sorted_note(arpeggio_index).note;
      arpeggio_index++;
    }
    step_enable[current_step] = 1;
    step_velocity[current_step] = INITIAL_VELOCITY;
  }
}

void advance() {
  sequencer_advance_without_play();
  sequencer_trigger_note();
}

void tick_clock() {
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

  if (running && (sequencer_clock % sequencer_divider) == 0) {
    advance();
  }
  sequencer_clock++;
}

void stop() {
  if (running) {

    usbMIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    MIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    usbMIDI.sendRealTime(midi::Stop);
    MIDI.sendRealTime(midi::Stop);

    running = false;
    sequencer_untrigger_note();
  }
  midi_clock = 0;
}

void init() {
  note_stack.Init();
  for (int i = 0; i < SEQUENCER_NUM_STEPS; i++) {
    step_note[i] = SCALE[random(9)];
  }
  tempo_handler.setHandleTempoEvent(tick_clock);
  tempo_handler.setHandleAlignEvent(align_clock);
  tempo_handler.setPPQN(PULSES_PER_QUARTER_NOTE);
  stop();
  current_step = SEQUENCER_NUM_STEPS - 1;
}

void reset_midi_clock() {
  midi_clock = 0;
  tempo_handler.midi_clock_reset();
}

void restart() {
  MIDI.sendRealTime(midi::Start);
  delay(1);
  current_step = SEQUENCER_NUM_STEPS - 1;
  reset_midi_clock();
  running = true;
  sequencer_clock = 0;
}

void sequencer_start() {
  MIDI.sendRealTime(midi::Continue);
  usbMIDI.sendRealTime(midi::Continue);
  reset_midi_clock();
  running = true;
}

void toggle_running() {
  if (running) {
    stop();
  } else {
    sequencer_start();
  }
}

void update() {
  gate_length_msec = map(synth.gateLength, 0, 1023, 10, 200);
  tempo_handler.update(midi_clock);

  if (!note_is_done_playing && millis() >= note_off_time && note_is_triggered) {
    sequencer_untrigger_note();
  }
}

void set_note(uint8_t note, uint8_t velocity) {
  note_stack.NoteOn(note, velocity);
}

void unset_note(uint8_t note) { note_stack.NoteOff(note); }

static uint8_t n = 255;
static uint8_t s = 255;

void keyboard_to_note() {
  const uint8_t k = note_stack.most_recent_note().note;

  if (true) {
    if (note_stack.size() != s) {
      s = note_stack.size();
      if (s > 0) {
        if (k != n) {
          if (!running) {
            sequencer_advance_without_play();
          }
          note_on(k + transpose, INITIAL_VELOCITY, true);
          n = k;
        }
      } else {
        note_off();
        n = 255; // Make sure this is a non existing note in the scale
      }
    }
  }
}

void clear_all() {
  note_off();
  note_stack.Clear();
}

}; // namespace Sequencer

// Publically used:
void sequencer_init() { Sequencer::init(); }
void sequencer_restart() { Sequencer::restart(); }
void sequencer_stop() { Sequencer::stop(); }
void sequencer_update() { Sequencer::update(); }
void sequencer_toggle_start() { Sequencer::toggle_running(); }
void keyboard_set_note(uint8_t note) {
  Sequencer::set_note(note, Sequencer::INITIAL_VELOCITY);
}
void keyboard_set_note(uint8_t note, uint8_t velocity) {
  Sequencer::set_note(note, velocity);
}
void keyboard_unset_note(uint8_t note) { Sequencer::unset_note(note); }
void keyboard_to_note() { Sequencer::keyboard_to_note(); }
void sequencer_clear_all() { Sequencer::clear_all(); }
void sequencer_set_double_speed(bool val) {
  if (val && !Sequencer::running) {
    Sequencer::advance();
  }

  double_speed = val;
}

void sequencer_set_random(bool val) {
  Sequencer::next_step_is_random = val;

  if (val && !Sequencer::running) {
    Sequencer::advance();
  }

  Sequencer::random_flag = val;
}

bool sequencer_is_running() { return Sequencer::running; }
bool sequencer_toggle_step(const uint8_t step) {
  Sequencer::step_enable[step] = 1 - Sequencer::step_enable[step];
  Sequencer::step_velocity[step] = Sequencer::INITIAL_VELOCITY;
  return Sequencer::step_enable[step];
}

bool sequencer_step_enabled(const uint8_t index) {
  return Sequencer::step_enable[index];
}
const uint8_t sequencer_step_note(const uint8_t index) {
  return Sequencer::step_note[index];
}
#endif
