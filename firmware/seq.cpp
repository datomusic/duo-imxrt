#include "seq.h"

Sequencer::Sequencer(Callbacks callbacks) : callbacks(callbacks) {
  held_notes.Init();
}

void Sequencer::start() { running = true; }
void Sequencer::restart() {
  running = true;
  clock = 0;
  current_step = 0;
}

void Sequencer::stop() {
  if (running) {
    running = false;
    untrigger_note();
  }
}

void Sequencer::handle_active_note(const uint32_t delta_millis) {
  gate_dur += delta_millis;
  if (running) {
    const bool note_is_over =
        note_state == Playing && (gate_dur >= gate_length_msec);
    if (note_is_over) {
      untrigger_note();
    }
  }
}

uint8_t Sequencer::quantized_current_step() {
  if (!running ||
      (clock % Sequencer::TICKS_PER_STEP < Sequencer::TICKS_PER_STEP / 2)) {
    return current_step;
  } else {
    return (current_step + 1) % SEQUENCER_NUM_STEPS;
  }
}

void Sequencer::update_notes(const uint32_t delta_millis,
                             const uint8_t step_offset) {
  handle_active_note(delta_millis);

  const uint8_t recent_note = held_notes.most_recent_note().note;
  const uint8_t stack_size = held_notes.size();

  if (stack_size != last_stack_size) {
    if (stack_size > 0) {
      if (recent_note != last_note) {
        if (!running) {
          advance_without_play();
        }

        const bool single_note = stack_size == 1 && last_stack_size == 0;

        if (single_note) {
          const uint8_t step = quantized_current_step() + step_offset;
          record_note(step, recent_note);
          trigger_note(step);
        }

        last_note = recent_note;
      }
    } else {
      untrigger_note();
      last_note = 255; // Make sure this is a non existing note in the scale
    }
  }

  last_stack_size = stack_size;
}

void Sequencer::advance(const uint8_t step_offset) {
  advance_without_play();
  trigger_note(current_step + step_offset);
}

void Sequencer::advance_without_play() {
  if (note_state == Playing) {
    Sequencer::untrigger_note();
  }

  current_step++;
  current_step %= SEQUENCER_NUM_STEPS;

  // Sample keys
  const uint8_t note_count = held_notes.size();

  if (arpeggio_index >= note_count) {
    arpeggio_index = 0;
  }

  if (note_count > 0 && running) {
    record_note(current_step, held_notes.sorted_note(arpeggio_index).note);
    arpeggio_index++;
  }
}

void Sequencer::trigger_note(const uint8_t step) {
  gate_dur = 0;

  const unsigned index = step % SEQUENCER_NUM_STEPS;
  if (step_enable[index] && note_state == Idle) {
    callbacks.note_on(step_note[index], INITIAL_VELOCITY);
  }
  note_state = Playing;
}

void Sequencer::untrigger_note() {
  if (note_state == Playing) {
    callbacks.note_off();
  }
  note_state = Idle;
}

void Sequencer::record_note(const uint8_t step, const uint8_t note) {
  step_note[step] = note;
  step_enable[step] = true;
}

void Sequencer::align_clock() {
  // round sequencer_clock to the nearest 12
  if (clock % 12 > 6) {
    clock += 12 - (clock % 12);
  } else {
    clock -= (clock % 12);
  }
}
