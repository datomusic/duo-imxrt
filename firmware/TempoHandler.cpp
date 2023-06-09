#include "TempoHandler.h"

void TempoHandler::update(const uint32_t midi_clock) {
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
    tempo.update_internal(*this, synth_params.speed);
    break;
  case TEMPO_SOURCE_MIDI:
    update_midi(midi_clock);
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

  if ((_clock % _ppqn) == 0) {
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

void TempoHandler::update_midi(const uint32_t midi_clock) {
  if (midi_clock != _previous_midi_clock) {
    _previous_midi_clock = midi_clock;
    _previous_clock_time = micros();
    trigger();
  }
}

void TempoHandler::reset_clock_source() {
  _midi_clock_received_flag = 0;
  _source = TEMPO_SOURCE_INTERNAL;
}
