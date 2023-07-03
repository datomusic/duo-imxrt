#include "seq.h"

#define INITIAL_VELOCITY 100

Seq::Seq(Callbacks callbacks) : callbacks(callbacks) { active_notes.Init(); }

void Seq::start() { running = true; }
void Seq::restart() {
  running = true;
  current_step = SEQUENCER_NUM_STEPS - 1;
  clock = 0;
}

void Seq::stop() {
  if (running) {
    running = false;
    untrigger_note();
  }
}

void Seq::update(const uint32_t current_millis, const int gate_length_msec) {
  if (running) {
    const uint32_t note_off_time = previous_note_on_time + gate_length_msec;
    const bool note_active =
        note_state == Playing && current_millis >= note_off_time;
    if (note_active) {
      untrigger_note();
    }
  }
}

void Seq::keyboard_to_note(const uint32_t current_millis,
                           const uint8_t step_offset) {
  const uint8_t recent_note = active_notes.most_recent_note().note;
  const uint8_t stack_size = active_notes.size();

  if (stack_size != last_stack_size) {
    if (stack_size > 0) {
      if (recent_note != last_note) {
        if (!running) {
          advance_without_play();
        }

        const bool single_note = stack_size == 1 && last_stack_size == 0;
        uint8_t step = current_step;
        if (running &&
            (clock % PULSES_PER_EIGHT_NOTE >= PULSES_PER_EIGHT_NOTE / 2)) {
          step = (current_step + 1) % SEQUENCER_NUM_STEPS;
        }

        if (!running || single_note) {
          record_note(step, recent_note);
          trigger_step(step + step_offset, current_millis);
        }

        last_note = recent_note;
      }
    } else {
      callbacks.note_off();
      last_note = 255; // Make sure this is a non existing note in the scale
    }
  }

  last_stack_size = stack_size;
}

void Seq::untrigger_note() {
  note_state = Idle;
  callbacks.note_off();
}

void Seq::advance(const uint32_t current_millis, const uint8_t step_offset) {
  advance_without_play();
  trigger_step(current_step + step_offset, current_millis);
}

void Seq::advance_without_play() {
  static uint8_t arpeggio_index = 0;

  current_step++;
  current_step %= SEQUENCER_NUM_STEPS;

  // Sample keys
  const uint8_t note_count = active_notes.size();

  if (arpeggio_index >= note_count) {
    arpeggio_index = 0;
  }

  if (note_count > 0 && running) {
    record_note(current_step, active_notes.sorted_note(arpeggio_index).note);
    arpeggio_index++;
  }
}

void Seq::trigger_step(const uint8_t step, const uint32_t current_millis) {
  note_state = Playing;
  previous_note_on_time = current_millis;

  callbacks.note_on(step_note[((step) % SEQUENCER_NUM_STEPS)], INITIAL_VELOCITY,
                    step_enable[((step) % SEQUENCER_NUM_STEPS)]);
}

void Seq::activate_note(uint8_t note, uint8_t velocity) {
  active_notes.NoteOn(note, velocity);
}

void Seq::activate_note(uint8_t note) { activate_note(note, INITIAL_VELOCITY); }

void Seq::deactivate_note(uint8_t note) { active_notes.NoteOff(note); }

void Seq::record_note(const uint8_t step, const uint8_t note) {
  step_note[step] = note;
  step_enable[step] = true;
}

void Seq::align_clock() {
  // round sequencer_clock to the nearest 12
  if (clock % 12 > 6) {
    clock += 12 - (clock % 12);
  } else {
    clock -= (clock % 12);
  }
}
