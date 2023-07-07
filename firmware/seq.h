#ifndef SEQ_H_0PHDG2MB
#define SEQ_H_0PHDG2MB

#include "note_stack.h"
#include <cstdint>

struct Sequencer {
  static const uint8_t NUM_STEPS = 8;
  static const unsigned PULSES_PER_QUARTER_NOTE = 24;
  static const unsigned TICKS_PER_STEP = (PULSES_PER_QUARTER_NOTE / 2);
  static const unsigned INITIAL_VELOCITY = 100;

  static uint8_t wrapped_step(const uint8_t step) { return step % NUM_STEPS; }

  struct Callbacks {
    typedef void (&NoteOn)(uint8_t note, uint8_t velocity);
    typedef void (&NoteOff)(void);
    NoteOn note_on;
    NoteOff note_off;
  };

  enum SpeedModifier { NormalSpeed, HalfSpeed, DoubleSpeed };

  Sequencer(Callbacks callbacks);
  void start();
  void restart();
  void stop();
  void advance();
  bool tick_clock();
  void update_notes(uint32_t delta_millis);
  void align_clock();
  inline void hold_note(uint8_t note, uint8_t velocity) {
    held_notes.NoteOn(note, velocity);
  }
  inline void hold_note(uint8_t note) { hold_note(note, INITIAL_VELOCITY); }
  inline void release_note(uint8_t note) { held_notes.NoteOff(note); }
  inline void release_all_notes() { held_notes.Clear(); };
  inline bool is_running() const { return running; }
  inline uint8_t get_cur_step() const {
    return (current_step + step_offset) % NUM_STEPS;
  }
  inline uint64_t get_clock() const { return clock; }
  inline bool gate_active() const { return gate_dur <= gate_length_msec; }
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

  uint32_t gate_length_msec = 0;
  uint8_t step_offset = 0;
  SpeedModifier speed_mod = NormalSpeed;

private:
  void trigger_note(uint8_t step);
  void untrigger_note();
  void record_note(uint8_t step, uint8_t note);
  void update_gate(uint32_t delta_millis);
  uint8_t quantized_current_step();
  void step_arpeggiator();
  inline void inc_current_step() {
    current_step = wrapped_step(current_step + 1);
  }

  Callbacks callbacks;
  NoteStack<10> held_notes;
  bool running = false;
  uint8_t current_step = 0;
  uint64_t clock = 0;
  uint8_t arpeggio_index = 0;
  uint8_t last_stack_size = 0;

  enum NoteState { Idle, Playing };
  NoteState note_state = Idle;
  uint32_t gate_dur = 0;

  struct Step {
    uint8_t enabled = false;
    uint8_t note = 0;
  };

  Step steps[NUM_STEPS];
};

#endif /* end of include guard: SEQ_H_0PHDG2MB */
