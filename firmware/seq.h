#ifndef SEQ_H_0PHDG2MB
#define SEQ_H_0PHDG2MB

#include "note_stack.h"
#include <cstdint>

namespace Sequencer {

static const unsigned INITIAL_VELOCITY = 100;

struct Arpeggiator {
  void advance() {
    index++;
    if (index >= held_notes.size()) {
      index = 0;
    }
  }

  uint8_t count() const { return held_notes.size(); }
  inline void hold_note(uint8_t note, uint8_t velocity) {
    held_notes.NoteOn(note, velocity);
  }
  inline void release_note(uint8_t note) { held_notes.NoteOff(note); }
  inline void release_all_notes() { held_notes.Clear(); };

  uint8_t index = 0;
  NoteStack<10> held_notes;
};

struct Gate {
  void update(const uint32_t delta_millis) { elapsed_millis += delta_millis; }
  void trigger() { elapsed_millis = 0; }
  bool active() const { return elapsed_millis <= length_millis; }

  uint32_t length_millis = 1;

private:
  uint16_t elapsed_millis = 0;
};

struct Callbacks {
  typedef void (&NoteOn)(uint8_t note, uint8_t velocity);
  typedef void (&NoteOff)(void);
  NoteOn note_on;
  NoteOff note_off;
};

struct Output {
  Output(Callbacks callbacks) : callbacks(callbacks) {}
  void live_note_on(const uint8_t note) { on(note); }
  void live_note_off() {off();}
  void step_on(const uint8_t note) { on(note); }
  void step_off() { off(); }

  const Callbacks callbacks;
  bool playing = false;
  uint8_t note = 0;

private:
  void on(const uint8_t note) {
    if (playing && note != this->note) {
      off();
      callbacks.note_on(note, INITIAL_VELOCITY);
    } else if (!playing) {
      callbacks.note_on(note, INITIAL_VELOCITY);
    }

    playing = true;
    this->note = note;
  }

  void off() {
    if (playing) {
      callbacks.note_off();
      playing = false;
    }
  }
};

static const uint8_t NUM_STEPS = 8;
static const unsigned PULSES_PER_QUARTER_NOTE = 24;
static const unsigned TICKS_PER_STEP = (PULSES_PER_QUARTER_NOTE / 2);

enum SpeedModifier { NormalSpeed, HalfSpeed, DoubleSpeed };

struct Sequencer {

  static uint8_t wrapped_step(const uint8_t step) { return step % NUM_STEPS; }

  Sequencer(Callbacks callbacks);
  void start();
  void restart();
  void stop();
  void advance();
  bool tick_clock();
  void update_notes(uint32_t delta_millis);
  void align_clock();
  inline void hold_note(uint8_t note, uint8_t velocity) {
    arp.hold_note(note, velocity);
  }
  inline void release_note(uint8_t note) { arp.release_note(note); }
  inline void release_all_notes() { arp.release_all_notes(); };
  inline void hold_note(uint8_t note) { hold_note(note, INITIAL_VELOCITY); }
  inline bool is_running() const { return running; }
  inline uint8_t cur_step_index() const {
    return (current_step + step_offset) % NUM_STEPS;
  }
  inline uint32_t get_clock() const { return clock; }
  inline bool gate_active() const { return gate.active(); }
  inline uint8_t get_step_enabled(const uint8_t step) const {
    return steps[wrapped_step(step)].enabled;
  }
  inline uint8_t get_step_note(const uint8_t step) const {
    return steps[wrapped_step(step)].note;
  }
  inline bool toggle_step(uint8_t step) {
    step = wrapped_step(step);
    const auto enabled = !steps[step].enabled;
    steps[step].enabled = enabled;
    return enabled;
  }
  inline void set_step_note(const uint8_t step, const uint8_t note) {
    steps[wrapped_step(step)].note = note;
  }

  uint8_t step_offset = 0;
  SpeedModifier speed_mod = NormalSpeed;

  void set_gate_length(const uint32_t millis) { gate.length_millis = millis; }

private:
  void record_note(uint8_t step, uint8_t note);
  void advance_running();
  uint8_t quantized_current_step();
  inline void inc_current_step() {
    current_step = wrapped_step(current_step + 1);
  }

  bool running = false;
  bool step_triggered = false;
  uint8_t current_step = 0;
  uint8_t last_stack_size = 0;
  uint32_t clock = 0;
  Arpeggiator arp;
  Gate gate;
  Output output;

  struct Step {
    uint8_t enabled = false;
    uint8_t note = 0;
  };
  Step steps[NUM_STEPS];
  Step manual_note;
};
} // namespace Sequencer

#endif /* end of include guard: SEQ_H_0PHDG2MB */
