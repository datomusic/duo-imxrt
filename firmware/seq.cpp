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
    playing_note.off();
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
  gate.update(delta_millis);
  playing_note.update(gate, running);

  const uint8_t stack_size = held_notes.size();
  const uint8_t step = quantized_current_step() + step_offset;
  const uint8_t recent_note = held_notes.most_recent_note().note;

  if (stack_size != last_stack_size) {
    if (stack_size == 0) {
      if (!running) {
        manual_note.enabled = false;
      }
    } else if (stack_size == 1 && last_stack_size == 0) {
      manual_note.enabled = true;
      manual_note.note = recent_note;
      record_note(step, recent_note);
      if (!running) {
        inc_current_step();
      }
    }
  }

  last_stack_size = stack_size;

  const Step cur_step = steps[wrapped_step(current_step + step_offset)];

  if (manual_note.enabled) {
    playing_note.on(manual_note.note);
    if (running) {
      manual_note.enabled = false;
    }
  } else if (cur_step.enabled && running) {
    if (!step_triggered) {
      playing_note.on(cur_step.note);
    }
    step_triggered = true;
  } else {
    playing_note.off();
  }
}

void Sequencer::advance() {
  if (running) {
    advance_running();
  } else {
    advance_stopped();
  }
}

void Sequencer::advance_running() {
  step_triggered = false;
  playing_note.off();
  inc_current_step();
  manual_note.enabled = false;
  arpeggio_index++;

  if (arpeggio_index >= held_notes.size()) {
    arpeggio_index = 0;
  }

  if (held_notes.size() > 0) {
    const auto note = held_notes.sorted_note(arpeggio_index).note;
    record_note(current_step, note);
  }
}

void Sequencer::advance_stopped() {
  step_triggered = false;
  playing_note.off();
  inc_current_step();
  manual_note.enabled = false;
  arpeggio_index++;
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
  clock++;

  if (running) {
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

    const bool should_advance = (clock % divider) == 0;
    if (should_advance) {
      advance();
    }

    return should_advance;
  } else {
    return false;
  }
}
