#ifndef Sequencer_h
#define Sequencer_h
/*
 The sequencer holds a number of notes
 Note timing is divided into 24 steps per quarter note
 */

#include <cstdint>

uint32_t last_sequencer_update;
bool double_speed = false;

void sequencer_update() {
  unsigned speed_mod = (unsigned)Sequencer::NormalSpeed;

  if (!tempo_handler.is_clock_source_internal()) {
    if (synth.speed > 900) {
      speed_mod += 1;
    } else if (synth.speed < 127) {
      speed_mod -= 1;
    }
  }

  if (double_speed) {
    speed_mod += 1;
  }

  sequencer.speed_mod = (Sequencer::SpeedModifier)speed_mod;

  sequencer.set_gate_length(map(synth.gateLength, 0, 1023, 10, 200) * 1000);
  tempo_handler.update(synth.speed);

  const uint32_t cur_micros = micros();
  const uint32_t delta = cur_micros - last_sequencer_update;
  last_sequencer_update = cur_micros;
  sequencer.update_gate(delta);
}

void sequencer_stop() {
  if (sequencer.is_running()) {
    MIDI::sendControlChange(123, 0, MIDI_CHANNEL);
    MIDI::sendRealTime(midi::Stop);
  }

  sequencer.stop();
}

void sequencer_start() {
  MIDI::sendRealTime(midi::Continue);

  if (tempo_handler.is_clock_source_internal()) {
    tempo_handler.reset_tempo();
  }

  sequencer.run();
}

void sequencer_start_from_MIDI() {
  MIDI::sendRealTime(midi::Start);
  delay(1);
  tempo_handler.set_MIDI_source();
  sequencer.run();
}

void sequencer_toggle_start() {
  if (sequencer.is_running()) {
    sequencer_stop();
  } else {
    sequencer_start();
  }
}

static void sequencer_tick_clock() {
  sequencer.tick_clock();
}

static void sequencer_align_clock() {
  sequencer.align_clock();
}

static void sequencer_init() {
  last_sequencer_update = micros();
  for (int i = 0; i < Sequencer::NUM_STEPS; i++) {
    sequencer.set_step_note(i, SCALE[random(9)]);
  }

  tempo_handler.setHandleTempoEvent(sequencer_tick_clock);
  tempo_handler.setHandleAlignEvent(sequencer_align_clock);

  sequencer_stop();
  double_speed = false;
}

#define keyboard_set_note(note) sequencer.hold_note(note)
#define keyboard_unset_note(note) sequencer.release_note(note)

#endif
