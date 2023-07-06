#ifndef SEQ_H_0PHDG2MB
#define SEQ_H_0PHDG2MB

#include "note_stack.h"
#include <cstdint>

#define PULSES_PER_QUARTER_NOTE 24
#define PULSES_PER_EIGHT_NOTE PULSES_PER_QUARTER_NOTE / 2

#define SEQUENCER_NUM_STEPS 8

struct Sequencer {
  static const int INITIAL_VELOCITY = 100;

  struct Callbacks {
    typedef void (&NoteOn)(uint8_t note, uint8_t velocity);
    typedef void (&NoteOff)(void);
    NoteOn note_on;
    NoteOff note_off;
  };

  Sequencer(Callbacks callbacks);
  void start();
  void restart();
  void stop();
  void update_notes(uint32_t delta_millis, int note_len_millis, uint8_t step_offset);
  void advance(uint8_t step_offset);
  void align_clock();
  inline void hold_note(uint8_t note, uint8_t velocity) {
    held_notes.NoteOn(note, velocity);
  }
  inline void hold_note(uint8_t note) { hold_note(note, INITIAL_VELOCITY); }
  inline void release_note(uint8_t note) { held_notes.NoteOff(note); }
  inline void release_all_notes() { held_notes.Clear(); };
  inline bool is_running() { return running; }
  inline uint8_t get_cur_step() { return current_step; }
  inline uint64_t get_clock() { return clock; }
  inline void inc_clock() { clock++; }

  uint8_t step_note[SEQUENCER_NUM_STEPS] = {1, 0, 6, 9, 0, 4, 0, 5};
  uint8_t step_enable[SEQUENCER_NUM_STEPS] = {1, 0, 1, 1, 1, 1, 0, 1};

private:
  void advance_without_play();
  void trigger_note(uint8_t step);
  void untrigger_note();
  void record_note(uint8_t step, uint8_t note);
  void handle_active_note(uint32_t delta_millis, int note_len_millis);

  Callbacks callbacks;
  NoteStack<10> held_notes;
  bool running = false;
  uint64_t clock = 0;
  uint8_t current_step = SEQUENCER_NUM_STEPS - 1;
  uint8_t arpeggio_index = 0;

  uint8_t last_note = 255;
  uint8_t last_stack_size = 0;

  enum NoteState { Idle, Playing };
  NoteState note_state = Idle;
  int active_note_dur;
};

#endif /* end of include guard: SEQ_H_0PHDG2MB */
