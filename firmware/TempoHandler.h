/*
 * Generates 24 pulses per quarter note based on one of the
 * following sources:
    - MIDI timecode packets
    - Sync pulses
    - Internal clock generator

    If the clock source is the internal clock, the speed is
    determined by the position of the TEMPO_POT. Tempo range is
    40ms to 666ms

    After 12 pulses the sequencer advances
    The Volca sync pulses have to be interpolated

 * Calls a callback function every time the timer advances
 * Sends MIDI timecode packets
 * Sends sync pulses
 */

#ifndef TempoHandler_h
#define TempoHandler_h

#define TEMPO_SYNC_DIVIDER   (Sequencer::PULSES_PER_QUARTER_NOTE / 2)

#include <MIDI.h>
#include "lib/tempo.h"
#include "lib/sync.h"
#include "firmware/seq.h"
#include "lib/elapsedMillis.h"

void midi_send_realtime(const midi::MidiType message);
 
class TempoHandler 
{
  friend class Tempo;

  public:
    TempoHandler() {
      tempo.reset();
    }

    inline void setHandleTempoEvent(void (*fptr)()) {
      tTempoCallback = fptr;
    }
    inline void setHandleAlignEvent(void (*fptr)()) {
      tAlignCallback = fptr;
    }
    void update(const uint32_t synth_speed) {
      if(_source != TEMPO_SOURCE_SYNC && Sync::detect()){
        _source = TEMPO_SOURCE_SYNC;
      }

      switch(_source) {
        case TEMPO_SOURCE_INTERNAL:
          tempo.update_internal(*this, synth_speed);
          break;
        case TEMPO_SOURCE_SYNC:
          update_sync();
          break;
        case TEMPO_SOURCE_MIDI:
          break;
      }

      if(syncStart >= TEMPO_SYNC_DIVIDER) {
        Sync::write(LOW);
      }
    }

    void midi_clock_received() {
      if (_source != TEMPO_SOURCE_MIDI) {
        _source = TEMPO_SOURCE_MIDI;
      }

      _previous_clock_time = micros();
      trigger();
    }
    void reset_clock_source() {
      _source = TEMPO_SOURCE_INTERNAL;
      reset_tempo();
    }
    void reset_tempo(){
      tempo.reset();
      _previous_clock_time = micros();
      _clock = 0;
    }
    bool is_clock_source_internal() const {
      return _source == TEMPO_SOURCE_INTERNAL;
    }

    void set_MIDI_source(){
      _source = TEMPO_SOURCE_MIDI;
    }
  private:

    enum Source{
      TEMPO_SOURCE_MIDI,
      TEMPO_SOURCE_SYNC,
      TEMPO_SOURCE_INTERNAL
    };


    Tempo tempo;
    elapsedMillis syncStart;
    void (*tTempoCallback)();
    void (*tAlignCallback)();
    Source _source = TEMPO_SOURCE_INTERNAL;
    uint32_t _previous_clock_time = 0;
    uint32_t _previous_sync_time = 0;
    uint32_t _tempo_interval = 0;
    uint16_t _clock = 0;

    void update_sync() {
      if (!Sync::detect()) {
        reset_clock_source();
        return;
      }

      static uint8_t _sync_pin_previous_value = 1;
      const uint8_t _sync_pin_value = Sync::read();

      if(_sync_pin_previous_value && !_sync_pin_value) {
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
        if(micros() - _previous_clock_time > _tempo_interval) {
          if(_clock < TEMPO_SYNC_DIVIDER) {
            if (tTempoCallback != 0) {
              _previous_clock_time = micros();
              trigger();
            }
          }
        }
      }
      _sync_pin_previous_value = _sync_pin_value;
    }

    /*
     * Calls the callback, updates the clock and sends out MIDI/Sync pulses
     */
    void trigger() {
      midi_send_realtime(midi::Clock);

      if((_clock % Sequencer::PULSES_PER_QUARTER_NOTE) == 0) {
        _clock = 0;
      }
      if((_clock % TEMPO_SYNC_DIVIDER) == 0) {
        Sync::write(HIGH);
        syncStart = 0;
      } 
      if (tTempoCallback != 0) {
        tTempoCallback();
      }
      if (_source == TEMPO_SOURCE_MIDI) {
        _previous_clock_time = micros();
      }
      _clock++;
    }
};

#endif
