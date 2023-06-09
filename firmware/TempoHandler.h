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

#define TEMPO_SOURCE_INTERNAL 0
#define TEMPO_SOURCE_MIDI     1
#define TEMPO_SOURCE_SYNC     2
#define TEMPO_SYNC_DIVIDER   12
#define TEMPO_SYNC_PULSE_MS  12

#include <MIDI.h>
#include "lib/tempo.h"
#include "lib/sync.h"
#include "firmware/synth_params.h"
#include "lib/elapsedMillis.h"

void midi_send_realtime(const midi::MidiType message);
 
class TempoHandler 
{
  friend class Tempo;

  public:
    TempoHandler(synth_parameters &synth_params): synth_params(synth_params){
      tempo.reset();
    }

    inline void setHandleTempoEvent(void (*fptr)()) {
      tTempoCallback = fptr;
    }
    inline void setHandleAlignEvent(void (*fptr)()) {
      tAlignCallback = fptr;
    }
    void setPPQN(int ppqn) {
      _ppqn = ppqn;
    }
    void update(const uint32_t midi_clock);
      uint8_t new_source = _source;


      if (new_source != _source) {
        _source = new_source;
        _midi_clock_received_flag = 0;
        tempo.reset();


    void midi_clock_received() {
      _midi_clock_received_flag = 1;
    }
    void midi_clock_reset() {
      _previous_midi_clock = 0;
    }
    void reset_clock_source();
    bool is_clock_source_internal() {
      return _source == TEMPO_SOURCE_INTERNAL;
    }
  private:
    synth_parameters& synth_params;
    Tempo tempo;
    elapsedMillis syncStart;
    void (*tTempoCallback)();
    void (*tAlignCallback)();
    int pot_pin;
    uint8_t _source = 0;
    uint32_t _previous_clock_time;
    uint32_t _previous_sync_time;
    uint32_t _tempo_interval;
    bool _midi_clock_block = false;
    uint32_t _previous_midi_clock = 0;
    bool _midi_clock_received_flag = 0;
    uint16_t _clock = 0;
    uint16_t _ppqn = 24;


    void update_midi(const uint32_t midi_clock);
    void update_sync();

    /*
     * Calls the callback, updates the clock and sends out MIDI/Sync pulses
     */
    void trigger();
};

#endif
