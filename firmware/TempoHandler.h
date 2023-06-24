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

#include "lib/elapsedMillis.h"
#include "lib/tempo.h"
#include <MIDI.h>

void midi_send_realtime(const midi::MidiType message);

class TempoHandler {
  friend class Tempo;

public:
  TempoHandler();

  void setHandleTempoEvent(void (*fptr)());
  void setHandleAlignEvent(void (*fptr)());
  void update(const int speed);
  void midi_clock_received();
  void reset_clock() { sequencer_clock = 0; }
  bool tick_clock(const int speed, const bool double_speed);
  void restart();
  void stop();
  void start();
  uint32_t clock() { return sequencer_clock; }
  void align_clock();
  void reset_clock_source();

  static const int PPQN = 24;
  static const int PULSES_PER_EIGHT_NOTE = (PPQN / 2);

private:
  Tempo tempo;
  elapsedMillis syncStart;
  void (*tTempoCallback)();
  void (*tAlignCallback)();
  uint8_t _source = 0;
  uint32_t _previous_clock_time;
  uint32_t _previous_sync_time;
  uint32_t _tempo_interval;
  uint32_t _previous_midi_clock = 0;
  bool _midi_clock_received_flag = 0;
  uint16_t _clock = 0;
  uint32_t sequencer_clock = 0;
  uint32_t midi_clock = 0;

  void update_midi();
  void update_sync();
  void midi_clock_reset();

  /*
   * Calls the callback, updates the clock and sends out MIDI/Sync pulses
   */
  void trigger();
};

#endif
