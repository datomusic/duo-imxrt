#include "seq.h"

enum Zone { Early, Middle, Late };

static Zone get_zone(uint32_t clock) {
  const auto third = Sequencer::TICKS_PER_STEP / 3;
  clock = clock % Sequencer::TICKS_PER_STEP;
  if (clock <= third) {
    return Early;
  } else if (clock >= 2 * third) {
    return Late;
  } else {
    return Middle;
  }
}

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
  last_played_step = NUM_STEPS;
  step_played_live = false;
  start();
}

void Sequencer::stop() {
  if (running) {
    running = false;
    output.off();
  }
}

void Sequencer::update_gate(const uint32_t delta_millis) {
  step_gate.update(delta_millis);
  live_gate.update(delta_millis);

  if (running) {
    if (step_played_live) {
      if (!live_gate.open()) {
        output.off();
      }
    } else if (!step_gate.open()) {
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

  if (!step_played_live || !live_gate.open()) {
    output.off();
  }

  inc_current_step();

  const auto step_index = current_step + step_offset;
  const Step cur_step = steps[wrapped_step(step_index)];

  arp.advance();

  if (last_played_step != step_index) {
    step_played_live = false;
    if (arp.count() > 0) {
      const uint8_t note = arp.current_note();
      record_note(note, current_step + step_offset);
      output.on(note);
    } else if (cur_step.enabled) {
      output.on(cur_step.note);
    }
  }

  last_played_step = step_index;
}

void Sequencer::record_note(const uint8_t note, uint8_t step) {
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

void Sequencer::play_live_note(const uint8_t note, const uint8_t step) {
  output.on(note);
  step_played_live = true;
  live_gate.trigger();
  last_played_step = step;
}

void Sequencer::hold_note(uint8_t note, uint8_t velocity) {
  arp.hold_note(note, velocity);

  const auto active_note_count = arp.count();
  if (active_note_count > 0) {
    const auto arp_note = arp.recent_note();
    const auto zone = get_zone(clock);

    if (running) {
      bool play_note = false;
      bool rec_note = false;
      uint8_t rec_step = current_step + step_offset;

      switch (zone) {
      case Early: {
        rec_note = true;
        play_note = true;
      } break;
      case Late:
        rec_note = true;
        rec_step++;
        break;
      case Middle:
        play_note = true;
        break;
      }

      if (rec_note) {
        record_note(arp_note, rec_step);
      }

      if (play_note && active_note_count == 1) {
        play_live_note(arp_note, current_step + step_offset);
      }
    } else {
      record_note(arp_note, current_step + step_offset);
      output.on(arp_note);
      inc_current_step();
    }
  }
}

void Sequencer::release_note(uint8_t note) {
  arp.release_note(note);
  step_played_live = false;
  if (!running) {
    if (arp.count() > 0) {
      output.on(arp.recent_note());
    } else if (!running) {
      output.off();
    }
  }
}

} // namespace Sequencer
