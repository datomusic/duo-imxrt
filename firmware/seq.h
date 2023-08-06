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
  inline uint32_t get_clock() const { return clock; }
  inline bool gate_active() const { return playing_note.gate_active(); }
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

  void set_gate_length(const uint32_t gate) {
    playing_note.gate_length_msec = gate;
  }

private:
  void stop_playing_note();
  void record_note(uint8_t step, uint8_t note);
  uint8_t quantized_current_step();
  inline void inc_current_step() {
    current_step = wrapped_step(current_step + 1);
  }

  NoteStack<10> held_notes;
  bool running = false;
  uint8_t current_step = 0;
  uint32_t clock = 0;
  uint8_t arpeggio_index = 0;
  uint8_t last_stack_size = 0;

  uint16_t gate_dur = 0;

  struct ActiveNote {
    uint8_t enabled = false;
    uint8_t note = 0;
  };

  ActiveNote steps[NUM_STEPS];
  bool step_triggered = false;

  struct PlayingNote {
    PlayingNote(Callbacks callbacks) : callbacks(callbacks){};

    void update(const bool running, const uint32_t delta_millis) {
      gate_dur += delta_millis;
      if (running) {
        const bool gate_closed = (gate_dur >= gate_length_msec);
        if (gate_closed) {
          off();
        }
      }
    }

    void on(const uint8_t note) {
      if (playing && note != this->note) {
        off();
        callbacks.note_on(note, INITIAL_VELOCITY);
      } else if (!playing) {
        callbacks.note_on(note, INITIAL_VELOCITY);
      }

      gate_dur = 0;
      playing = true;
      this->note = note;
    }

    void off() {
      if (playing) {
        callbacks.note_off();
        playing = false;
      }
    }

    bool gate_active() const { return gate_dur <= gate_length_msec; }

    uint32_t gate_length_msec = 1;

  private:
    const Callbacks callbacks;
    bool playing = false;
    uint16_t gate_dur = 0;
    uint8_t note = 0;
  };

  ActiveNote cur_step_note;
  PlayingNote playing_note;

  bool manual_note_enabled = false;
  uint32_t manual_note_note = 0;
};

#endif /* end of include guard: SEQ_H_0PHDG2MB */
