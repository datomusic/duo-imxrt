#ifndef Sequencer_h
#define Sequencer_h
/*
 The sequencer holds a number of notes
 Note timing is divided into 24 steps per quarter note
 */

#include "firmware/note_stack.h"
#include <cstdint>

const uint8_t SEQUENCER_NUM_STEPS = 8;

// Initial sequencer values

uint32_t previous_note_on_time = 0;
uint32_t sequencer_clock = 0;
uint8_t current_step = 0;
int random_offset = 0;

namespace Sequencer {
static const uint8_t INITIAL_VELOCITY = 100;
uint8_t step_enable[SEQUENCER_NUM_STEPS] = {1, 0, 1, 1, 1, 1, 0, 1};
uint8_t step_note[SEQUENCER_NUM_STEPS] = {1, 0, 6, 9, 0, 4, 0, 5};
uint8_t step_velocity[SEQUENCER_NUM_STEPS] = {100, 100, 100, 100,
                                              100, 100, 100, 100};

static bool note_is_done_playing = false;
static bool note_is_triggered = false;
static uint32_t note_off_time = 0;
static int gate_length_msec = 40;
static bool random_flag = 0;
static bool next_step_is_random = false;
static bool running = false;
static bool double_speed = false;

NoteStack<10> note_stack;

void tick_clock();
void align_clock();
void stop();
void advance();
void sequencer_trigger_note();
void sequencer_untrigger_note();

void init() {
  note_stack.Init();
  for (int i = 0; i < SEQUENCER_NUM_STEPS; i++) {
    step_note[i] = SCALE[random(9)];
  }
  tempo_handler.setHandleTempoEvent(tick_clock);
  tempo_handler.setHandleAlignEvent(align_clock);
  stop();
  current_step = SEQUENCER_NUM_STEPS - 1;
}

void restart() {
  MIDI.sendRealTime(midi::Start);
  delay(1);
  current_step = SEQUENCER_NUM_STEPS - 1;
  running = true;
  tempo_handler.restart();
}

void align_clock() {
  tempo_handler.align_clock();
}

void start() {
  MIDI.sendRealTime(midi::Continue);
  usbMIDI.sendRealTime(midi::Continue);
  tempo_handler.start();
  running = true;
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
  tempo_handler.stop();
}

void toggle_running() {
  if (running) {
    stop();
  } else {
    start();
  }
}

void tick_clock() {
  const bool should_advance = tempo_handler.tick_clock(synth.speed, double_speed);
  if(running && should_advance){
    advance();
  }
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
  const uint8_t note_count = note_stack.size();

  if (arpeggio_index >= note_count) {
    arpeggio_index = 0;
  }

  if (note_count > 0) {
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

void update() {
  gate_length_msec = map(synth.gateLength, 0, 1023, 10, 200);
  tempo_handler.update(synth.speed);

  if (!note_is_done_playing && millis() >= note_off_time && note_is_triggered) {
    sequencer_untrigger_note();
  }
}

void sequencer_trigger_note() {
  note_is_triggered = true;
  note_is_done_playing = false;
  previous_note_on_time = millis();
  note_off_time = previous_note_on_time + gate_length_msec;

  step_velocity[current_step] = INITIAL_VELOCITY;

  note_on(step_note[((current_step + random_offset) % SEQUENCER_NUM_STEPS)] +
              transpose,
          step_velocity[((current_step + random_offset) % SEQUENCER_NUM_STEPS)],
          step_enable[((current_step + random_offset) % SEQUENCER_NUM_STEPS)]);
}

void sequencer_untrigger_note() {
  note_is_done_playing = true;
  note_off();
  note_is_triggered = false;
  note_off_time =
      millis() + tempo_interval -
      gate_length_msec; // Set note off time to sometime in the future
}

void set_note(uint8_t note, uint8_t velocity) {
  note_stack.NoteOn(note, velocity);
}

void unset_note(uint8_t note) { note_stack.NoteOff(note); }

static uint8_t last_note = 255;
static uint8_t last_stack_size = 255;

void keyboard_to_note() {
  if (!running) {
    if (note_stack.size() != last_stack_size) {
      last_stack_size = note_stack.size();
      if (last_stack_size > 0) {
        const uint8_t k = note_stack.most_recent_note().note;
        if (k != last_note) {
          sequencer_advance_without_play();
          note_on(k + transpose, INITIAL_VELOCITY, true);
          last_note = k;
        }
      } else {
        note_off();
        last_note = 255; // Make sure this is a non existing note in the scale
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
void sequencer_start() { Sequencer::start(); }
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

  Sequencer::double_speed = val;
}

void sequencer_set_random(bool val) {
  Sequencer::next_step_is_random = val;

  if (val && !Sequencer::running) {
    Sequencer::advance();
  }

  Sequencer::random_flag = val;
}

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
