#include "seq.h"

namespace Sequencer {

Sequencer::Sequencer(Callbacks callbacks) : output(callbacks) {
  arp.held_notes.Init();

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
    output.step_off();
  }
}

uint8_t Sequencer::quantized_current_step() {
  if (!running || (clock % TICKS_PER_STEP < TICKS_PER_STEP / 2)) {
    return current_step;
  } else {
    return current_step + 1;
  }
}

void Sequencer::update_notes(const uint32_t delta_millis) {
  gate.update(delta_millis);
  if (running && !gate.active()) {
    output.step_off();
  }

  const uint8_t stack_size = arp.held_notes.size();
  const uint8_t step = quantized_current_step() + step_offset;
  const uint8_t recent_note = arp.held_notes.most_recent_note().note;

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
    output.live_note_on(manual_note.note);
    if (running) {
      manual_note.enabled = false;
    }
  }else{
    output.live_note_off();
  }

  if (running) {
    if (cur_step.enabled) {
      if (!step_triggered) {
        output.step_on(cur_step.note);
      }
      step_triggered = true;
    } else {
      output.step_off();
    }
  }
}

void Sequencer::advance() {
  if (running) {
    advance_running();
  } else {
    inc_current_step();
  }
}

void Sequencer::advance_running() {
  gate.trigger();
  step_triggered = false;
  output.step_off();
  inc_current_step();
  manual_note.enabled = false;
  arp.advance();

  if (arp.count() > 0) {
    const auto note = arp.held_notes.sorted_note(arp.index).note;
    record_note(current_step, note);
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

    const bool should_advance = running && (clock % divider) == 0;
    if (should_advance) {
      advance_running();
    }

    return should_advance;
  } else {
    return false;
  }
}

} // namespace Sequencer
