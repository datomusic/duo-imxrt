#include "seq.h"

Sequencer::Sequencer(Callbacks callbacks) : playing_note(callbacks) {
  held_notes.Init();
  for (int i = 0; i < NUM_STEPS; ++i) {
    steps[i].enabled = i != 1 && i != (NUM_STEPS - 2);
  }
}

void Sequencer::start() { running = true; }
void Sequencer::restart() {
  clock = 0;
  current_step = 0;
  start();
}

void Sequencer::stop() {
  if (running) {
    running = false;
    manual_note.enabled = false;
    manual_note.enabled = false;
    playing_note.off();
  }
}

void Sequencer::update_gate(const uint32_t delta_millis) {
  gate_dur += delta_millis;

  if (running) {
    const bool gate_closed = (gate_dur > gate_length_msec);
    if (gate_closed) {
      step_note.enabled = false;
    }
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

  if (!running && held_notes.size() == 0) {
    manual_note.enabled = false;
  }

  const uint8_t recent_note = held_notes.most_recent_note().note;
  const uint8_t stack_size = held_notes.size();

  if (stack_size != last_stack_size) {
    const bool single_note = stack_size == 1 && last_stack_size == 0;

    if (single_note) {
      const uint8_t step = quantized_current_step() + step_offset;
      record_note(step, recent_note);
      manual_note.enabled = true;
      manual_note.note = recent_note;
      if (!running) {
        inc_current_step();
      }
    }
  }

  last_stack_size = stack_size;

  if (step_note.enabled) {
    playing_note.on(step_note.note);
  } else if (manual_note.enabled) {
    playing_note.on(manual_note.note);
  } else {
    playing_note.off();
  }
}

void Sequencer::advance() {
  playing_note.off();
  inc_current_step();
  gate_dur = 0;
  step_note = steps[wrapped_step(current_step + step_offset)];

  if (running) {
    step_arpeggiator();

    if (held_notes.size() > 0) {
      record_note(current_step, held_notes.sorted_note(arpeggio_index).note);
    }
  }
}

void Sequencer::step_arpeggiator() {
  arpeggio_index++;
  if (arpeggio_index >= held_notes.size()) {
    arpeggio_index = 0;
  }
}

void Sequencer::record_note(uint8_t step, const uint8_t note) {
  step = wrapped_step(step);
  steps[step].enabled = true;
  steps[step].note = note;
}

void Sequencer::align_clock() {
  // round sequencer_clock to the nearest 12
  if (clock % 12 > 6) {
    clock += 12 - (clock % 12);
  } else {
    clock -= (clock % 12);
  }
}

bool Sequencer::tick_clock() {
  uint8_t divider = TICKS_PER_STEP;
  switch (speed_mod) {
  case HalfSpeed:
    divider *= 2;
    break;
  case DoubleSpeed:
    divider /= 2;
    break;
  case NormalSpeed:
    break;
  }

  clock++;

  const bool should_advance = running && (clock % divider) == 0;
  if (should_advance) {
    advance();
  }

  return should_advance;
}
