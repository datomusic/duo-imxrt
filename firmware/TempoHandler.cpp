#include "TempoHandler.h"
#include "lib/sync.h"
#include <cstdint>

#define TEMPO_SOURCE_INTERNAL 0
#define TEMPO_SOURCE_MIDI 1
#define TEMPO_SOURCE_SYNC 2
#define TEMPO_SYNC_DIVIDER 12
#define TEMPO_SYNC_PULSE_MS 12

TempoHandler::TempoHandler() { tempo.init(); }

void TempoHandler::update(const int speed) {
  // Determine which source is selected for tempo
  if (Sync::detect()) {
    if (_source != TEMPO_SOURCE_SYNC) {
      _source = TEMPO_SOURCE_SYNC;
    }
    _midi_clock_received_flag = 0;
  } else if (_midi_clock_received_flag) {
    if (_source != TEMPO_SOURCE_MIDI) {
      _source = TEMPO_SOURCE_MIDI;
    }
  } else {
    if (_source != TEMPO_SOURCE_INTERNAL) {
      _source = TEMPO_SOURCE_INTERNAL;
    }
  }

  switch (_source) {
  case TEMPO_SOURCE_INTERNAL:
    tempo.update_internal(*this, speed);
    break;
  case TEMPO_SOURCE_MIDI:
    update_midi();
    break;
  case TEMPO_SOURCE_SYNC:
    update_sync();
    break;
  }

  if (syncStart >= TEMPO_SYNC_PULSE_MS) {
    Sync::write(LOW);
  }
}

void TempoHandler::update_sync() {
  static uint8_t _sync_pin_previous_value = 1;
  uint8_t _sync_pin_value = Sync::read();

  if (_sync_pin_previous_value && !_sync_pin_value) {
    _tempo_interval = (micros() - _previous_sync_time) / TEMPO_SYNC_DIVIDER;
    _clock = 0;
    _previous_sync_time = micros();
    _previous_clock_time = micros();
    if (tAlignCallback != 0) {
      tAlignCallback();
    }
    if (tTempoCallback != 0) {
      trigger();
    }
  } else {
    if (micros() - _previous_clock_time > _tempo_interval) {
      if (_clock < TEMPO_SYNC_DIVIDER) {
        if (tTempoCallback != 0) {
          _previous_clock_time = micros();
          trigger();
        }
      }
    }
  }
  _sync_pin_previous_value = _sync_pin_value;
}

void TempoHandler::trigger() {
  midi_send_realtime(midi::Clock);

  if ((_clock % PPQN) == 0) {
    _clock = 0;
  }
  if ((_clock % TEMPO_SYNC_DIVIDER) == 0) {
    Sync::write(HIGH);
    syncStart = 0;
  }
  if (tTempoCallback != 0) {
    tTempoCallback();
  }
  _clock++;
}

void TempoHandler::update_midi() {
  if (midi_clock != _previous_midi_clock) {
    _previous_midi_clock = midi_clock;
    _previous_clock_time = micros();
    trigger();
  }
}

void TempoHandler::midi_clock_received() {
  _midi_clock_received_flag = 1;
  midi_clock++;
}

void TempoHandler::setHandleAlignEvent(void (*fptr)()) {
  tAlignCallback = fptr;
}

void TempoHandler::setHandleTempoEvent(void (*fptr)()) {
  tTempoCallback = fptr;
}

bool TempoHandler::tick_clock(const bool double_speed) {
  uint8_t sequencer_divider = TempoHandler::PULSES_PER_EIGHT_NOTE;
  if (double_speed) {
    sequencer_divider = TempoHandler::PULSES_PER_EIGHT_NOTE / 2;
  }

  /*
   * TODO: Handle doubling and halving of external tempo.
  if (_source != TEMPO_SOURCE_INTERNAL) {
    int potvalue = synth.speed;
    if (potvalue > 900) {
      sequencer_divider /= 2;
    } else if (potvalue < 127) {
      sequencer_divider *= 2;
    }
  }
  */

  const bool ret = (sequencer_clock % sequencer_divider) == 0;
  sequencer_clock++;

  return ret;
}

void TempoHandler::midi_clock_reset() {
  _previous_midi_clock = 0;
  midi_clock = 0;
}

void TempoHandler::stop() { midi_clock_reset(); }
void TempoHandler::start() { midi_clock_reset(); }

void TempoHandler::restart() {
  sequencer_clock = 0;
  midi_clock_reset();
}
