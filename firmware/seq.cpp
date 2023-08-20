#include "seq.h"

namespace Sequencer {

Sequencer::Sequencer(Callbacks callbacks) : output(callbacks) {
  arp.init();

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
    output.off();
  }
}

uint8_t Sequencer::quantized_current_step() {
  if (!running || (clock % TICKS_PER_STEP < TICKS_PER_STEP / 2)) {
    return current_step;
  } else {
    return current_step + 1;
  }
}

void Sequencer::update_gate(const uint32_t delta_millis) {
  step_gate.update(delta_millis);
  live_gate.update(delta_millis);

  if (running && !step_gate.open()) {
    if (!step_gate.open() && !live_gate.open()) {
      output.off();
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
  step_gate.trigger();

  output.off();
  output.off();
  inc_current_step();

  const auto step_index = current_step + step_offset;
  const Step cur_step = steps[wrapped_step(step_index)];

  if (arp.count() > 0) {
    arp.advance();
    const uint8_t note = arp.current_note();
    record_note(current_step + step_offset, note);
    output.on(note);
  } else if (last_recorded_step != step_index && cur_step.enabled) {
    output.on(cur_step.note);
  }

  last_recorded_step = -1;
}

void Sequencer::record_note(uint8_t step, const uint8_t note) {
  step = wrapped_step(step);
  steps[step].enabled = true;
  steps[step].note = note;
  last_recorded_step = step;
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

void Sequencer::hold_note(uint8_t note, uint8_t velocity) {
  arp.hold_note(note, velocity);
  const auto arp_note = arp.recent_note();

  record_note(quantized_current_step() + step_offset, arp_note);

  if (running) {
    if (arp.count() == 1) {
      output.on(arp_note);
      live_gate.trigger();
    }
  } else {
    output.on(arp_note);
    inc_current_step();
  }
}

void Sequencer::release_note(uint8_t note) {
  arp.release_note(note);
  if (arp.count() > 0) {
    output.on(arp.current_note());
  } else if (!running) {
    output.off();
  }
}

} // namespace Sequencer
