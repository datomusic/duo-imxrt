#ifndef SEQ_H_0PHDG2MB
#define SEQ_H_0PHDG2MB

#include "note_stack.h"
#include <cstdint>

#define PULSES_PER_QUARTER_NOTE 24
#define PULSES_PER_EIGHT_NOTE PULSES_PER_QUARTER_NOTE / 2

#define SEQUENCER_NUM_STEPS 8

struct Sequencer {
  struct Callbacks {
    typedef void (&NoteOn)(uint8_t midi_note, uint8_t velocity, bool enabled);
    typedef void (&NoteOff)(void);
    NoteOn note_on;
    NoteOff note_off;
  };

  Sequencer(Callbacks callbacks);
  void start();
  void restart();
  void stop();
  void update(uint32_t current_millis, int gate_length_msec);
  void keyboard_to_note(uint32_t current_millis, uint8_t step_offset);
  void advance(uint32_t current_millis, uint8_t step_offset);
  void activate_note(uint8_t note);
  void activate_note(uint8_t note, uint8_t velocity);
  void deactivate_note(uint8_t note);
  void deactive_all_notes() { active_notes.Clear(); };
  void align_clock();

  inline bool is_running() { return running; }
  inline uint8_t get_cur_step() { return current_step; }
  inline uint64_t get_clock() { return clock; }
  inline void inc_clock() { clock++; }

  uint8_t step_note[SEQUENCER_NUM_STEPS] = {1, 0, 6, 9, 0, 4, 0, 5};
  uint8_t step_enable[SEQUENCER_NUM_STEPS] = {1, 0, 1, 1, 1, 1, 0, 1};

private:
  void advance_without_play();
  void trigger_step(uint8_t step, uint32_t current_millis);
  void record_note(uint8_t step, uint8_t note);
  void untrigger_note();

  Callbacks callbacks;
  NoteStack<10> active_notes;
  bool running = false;
  uint32_t previous_note_on_time;
  uint64_t clock = 0;
  uint8_t current_step = SEQUENCER_NUM_STEPS - 1;

  uint8_t last_note = 255;
  uint8_t last_stack_size = 255;

  enum NoteState { Idle, Playing };
  NoteState note_state = Idle;
};

#endif /* end of include guard: SEQ_H_0PHDG2MB */