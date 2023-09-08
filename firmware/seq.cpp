#include "seq.h"

enum Zone { Early, Middle, Late };

namespace Sequencer {

static uint8_t divider_from_speed_mod(const SpeedModifier mod) {
  unsigned ret = TICKS_PER_STEP;
  switch (mod) {
  case HalfSpeed:
    ret *= 2;
    break;
  case DoubleSpeed:
    ret /= 2;
    break;
  case QuadSpeed:
    ret /= 4;
    break;
  default:
    break;
  }

  return ret;
}

static Zone get_zone(uint32_t clock, const SpeedModifier speed_mod) {
  const uint32_t ticks = divider_from_speed_mod(speed_mod);
  const uint32_t third = ticks / 3;
  clock = clock % ticks;

  if (clock <= third) {
    return Early;
  } else if (clock >= 2 * third) {
    return Late;
  } else {
    return Middle;
  }
}

Sequencer::Sequencer(Callbacks callbacks) : output(callbacks) {
  arp.init();

  for (int i = 0; i < NUM_STEPS; ++i) {
    steps[i].enabled = i != 1 && i != (NUM_STEPS - 2);
  }
}

void Sequencer::start() {
  running = true;
  const uint8_t divider = divider_from_speed_mod(speed_mod);
  const uint8_t step_index = current_step + step_offset;
  const Step cur_step = steps[wrapped_step(step_index)];
  if (cur_step.enabled && (clock % divider) == 0) {
    step_gate.trigger();
    output.on(cur_step.note);
    last_played_step = step_index;
  }
}

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

void Sequencer::update_gate(const uint32_t delta_micros) {
  step_gate.update(delta_micros);
  live_gate.update(delta_micros);

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
    step_gate.trigger();
  }
}

void Sequencer::advance_running() {
  step_gate.trigger();

  if (!step_played_live || !live_gate.open()) {
    output.off();
  }

  inc_current_step();

  const uint8_t step_index = current_step + step_offset;
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
  const unsigned remainder = clock % 12;
  if (remainder > 6) {
    const unsigned ticks = 12 - remainder;
    for (unsigned i = 0; i < ticks; ++i) {
      tick_clock();
    }
  } else {
    clock -= remainder;
  }
}

bool Sequencer::tick_clock() {
  clock++;

  if (running) {
    const uint8_t divider = divider_from_speed_mod(speed_mod);
    const bool should_advance = (clock % divider) == 0;
    if (should_advance) {
      advance_running();
    }

    return should_advance;
  } else {
    return false;
  }
}

void Sequencer::hold_note(const uint8_t note, const uint8_t velocity) {
  arp.hold_note(note, velocity);

  const uint8_t active_note_count = arp.count();
  if (active_note_count > 0) {
    const uint8_t arp_note = arp.recent_note();
    const Zone zone = get_zone(clock, speed_mod);
    if (running) {
      const uint8_t rec_step = current_step + step_offset;

      if ((zone == Early || zone == Middle) && active_note_count == 1) {
        output.on(note);
        step_played_live = true;
        live_gate.trigger();
        last_played_step = rec_step;
      }

      if (zone == Early) {
        record_note(arp_note, rec_step);
      } else if (zone == Late) {
        record_note(arp_note, rec_step + 1);
      }
    } else {
      record_note(arp_note, current_step + step_offset);
      output.on(arp_note);
      inc_current_step();
    }
  }
}

void Sequencer::release_note(const uint8_t note) {
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
