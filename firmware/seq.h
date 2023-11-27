#ifndef SEQ_H_0PHDG2MB
#define SEQ_H_0PHDG2MB

#include "note_stack.h"
#include <cstdint>

namespace Sequencer {
static const unsigned INITIAL_VELOCITY = 100;
static const uint8_t NUM_STEPS = 8;
static const unsigned PULSES_PER_QUARTER_NOTE = 24;
static const unsigned TICKS_PER_STEP = (PULSES_PER_QUARTER_NOTE / 2);

struct Arpeggiator {
  void init() {
    held_notes.Init();
    index = 0;
  }

  void advance() {
    index++;
  }

  uint8_t recent_note() const {
    return held_notes.most_recent_note().note;
  }
  uint8_t current_note() {
    index = index % held_notes.size();

    return held_notes.sorted_note(index).note;
  }
  uint8_t count() const {
    return held_notes.size();
  }
  inline void hold_note(uint8_t note, uint8_t velocity) {
    if (count() > 0 && note < current_note()) {
      index++;
    }

    held_notes.NoteOn(note, velocity);
  }
  inline void release_note(uint8_t note) {
    held_notes.NoteOff(note);
  }
  inline void release_all_notes() {
    held_notes.Clear();
  };

private:
  uint8_t index = 0;
  NoteStack<10> held_notes;
};

struct Gate {
  void update(const uint32_t delta_millis) {
    elapsed += delta_millis;
  }
  void trigger() {
    elapsed = 0;
  }
  bool open() const {
    return elapsed <= length;
  }

  uint32_t length = 1;

private:
  uint32_t elapsed = 0;
};

struct Output {
  struct Callbacks {
    typedef void (&NoteOn)(uint8_t note, uint8_t velocity);
    typedef void (&NoteOff)(void);
    NoteOn note_on;
    NoteOff note_off;
  };

  Output(Callbacks callbacks) : callbacks(callbacks) {
  }

  void on(const uint8_t note) {
    if (output_active) {
      off();
    }

    output_active = true;
    active_note = note;
    callbacks.note_on(active_note, INITIAL_VELOCITY);
  }

  void off() {
    if (output_active) {
      callbacks.note_off();
      output_active = false;
    }
  }

  bool active() const {
    return output_active;
  }

private:
  const Callbacks callbacks;
  bool output_active = false;
  uint8_t active_note = 0;
};

enum SpeedModifier {
  HalfSpeed = 1,
  NormalSpeed = 2,
  DoubleSpeed = 3,
  QuadSpeed = 4
};

static uint8_t wrapped_step(const uint8_t step) {
  return step % NUM_STEPS;
}

struct Sequencer;
typedef void (&OnRunnningAdvance)(Sequencer &);

struct Sequencer {
  Sequencer(Output::Callbacks callbacks, OnRunnningAdvance on_running_advance);
  void set_running(bool running);
  void reset();
  void advance();
  void tick_clock();
  void update_gate(uint32_t delta_micros);
  void align_clock();
  inline void hold_note(uint8_t note) {
    hold_note(note, INITIAL_VELOCITY);
  }
  void hold_note(uint8_t note, uint8_t velocity);
  void release_note(uint8_t note);
  inline void release_all_notes() {
    arp.release_all_notes();
    if (!running) {
      output.off();
    }
  };
  inline bool is_running() const {
    return running;
  }
  inline uint8_t cur_step_index() const {
    return (current_step + step_offset) % NUM_STEPS;
  }
  inline uint32_t get_clock() const {
    return clock;
  }
  inline bool gate_active() const {
    return step_gate.open();
  }
  inline bool note_playing() const {
    return output.active();
  }
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

  void set_gate_length(const uint32_t micros) {
    step_gate.length = live_gate.length = micros;
  }

  void set_step_offset(uint8_t offset);
  uint8_t get_step_offset() const {
    return step_offset;
  }

  SpeedModifier speed_mod = NormalSpeed;

private:
  void record_note(uint8_t note, uint8_t step);
  void advance_running();
  void play_current_step();
  uint8_t quantized_current_step();
  inline void inc_current_step() {
    current_step = wrapped_step(current_step + 1);
  }

  bool running = false;
  bool step_played_live = false;
  uint8_t current_step = 0;
  uint8_t step_offset = 0;
  uint8_t last_played_step = 0;
  uint32_t clock = 0;
  Arpeggiator arp;
  Gate step_gate;
  Gate live_gate;
  Output output;
  OnRunnningAdvance on_running_advance;

  struct Step {
    uint8_t enabled = false;
    uint8_t note = 0;
  };
  Step steps[NUM_STEPS];
};
} // namespace Sequencer

#endif /* end of include guard: SEQ_H_0PHDG2MB */
