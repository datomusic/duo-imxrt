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

void Sequencer::update_gate(const uint32_t delta_millis) {
  gate_dur += delta_millis;
  const bool note_is_over = (gate_dur >= gate_length_msec);
  if (note_is_over && held_notes.size() == 0) {
    untrigger_note();
  }
}

uint8_t Sequencer::quantized_current_step() {
  if (!running ||
      (clock % Sequencer::TICKS_PER_STEP < Sequencer::TICKS_PER_STEP / 2)) {
    return current_step;
  } else {
    return current_step + 1;
  }
}

void Sequencer::update_notes(const uint32_t delta_millis) {
  update_gate(delta_millis);

  const uint8_t recent_note = held_notes.most_recent_note().note;
  const uint8_t stack_size = held_notes.size();

  if (stack_size != last_stack_size) {
    if (stack_size == 0) {
      untrigger_note();
    } else {
      const bool single_note = stack_size == 1 && last_stack_size == 0;

      if (single_note) {
        const uint8_t step = quantized_current_step() + step_offset;
        record_note(step, recent_note);
        trigger_note(step);
      }

      if (!running) {
        inc_current_step();
      }
    }
  }

  last_stack_size = stack_size;
}

void Sequencer::advance() {
  if (note_state == Playing) {
    untrigger_note();
  }

  inc_current_step();

  if (running) {
    step_arpeggiator();

    if (held_notes.size() > 0) {
      record_note(current_step, held_notes.sorted_note(arpeggio_index).note);
    }
  }

  trigger_note(current_step + step_offset);
}

void Sequencer::step_arpeggiator() {
  arpeggio_index++;
  if (arpeggio_index >= held_notes.size()) {
    arpeggio_index = 0;
  }
}

void Sequencer::trigger_note(uint8_t step_index) {
  if (note_state == Idle) {
    step_index = wrapped_step(step_index);
    const Step step = steps[step_index];
    if (step.enabled) {
      callbacks.note_on(step.note, INITIAL_VELOCITY);
    }

    note_state = Playing;
    gate_dur = 0;
  }
}

void Sequencer::untrigger_note() {
  if (note_state == Playing) {
    callbacks.note_off();
  }
  note_state = Idle;
}

void Sequencer::record_note(const uint8_t step, const uint8_t note) {
  steps[wrapped_step(step)].enable_note(note);
}

void Sequencer::align_clock() {
  // round sequencer_clock to the nearest 12
  if (clock % 12 > 6) {
    clock += 12 - (clock % 12);
  } else {
    clock -= (clock % 12);
  }
}
