#ifndef Sequencer_h
#define Sequencer_h
/*
 The sequencer holds a number of notes
 Note timing is divided into 24 steps per quarter note
 */

uint32_t last_sequencer_update;
bool double_speed = false;

static void sequencer_update() {
  sequencer.set_gate_length(map(synth.gateLength, 0, 1023, 10, 200));
  tempo_handler.update();

  const uint32_t cur_millis = millis();
  const uint32_t delta = cur_millis - last_sequencer_update;
  last_sequencer_update = cur_millis;
  sequencer.update_gate(delta);
}

static void reset_midi_clock() {
  tempo_handler.midi_clock_reset();
}

static void sequencer_stop() {
  if (sequencer.is_running()) {
    usbMIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    MIDI.sendControlChange(123, 0, MIDI_CHANNEL);
    usbMIDI.sendRealTime(midi::Stop);
    MIDI.sendRealTime(midi::Stop);
  }

  sequencer.stop();
  tempo_handler.midi_clock_reset();
}

static void sequencer_start() {
  MIDI.sendRealTime(midi::Continue);
  usbMIDI.sendRealTime(midi::Continue);
  reset_midi_clock();
  sequencer.start();
}

static void sequencer_toggle_start() {
  if (sequencer.is_running()) {
    sequencer_stop();
  } else {
    sequencer_start();
  }
}

static void sequencer_tick_clock() {
  if (!tempo_handler.is_clock_source_internal()) {
    if (synth.speed > 900) {
      sequencer.speed_mod = Sequencer::DoubleSpeed;
    } else if (synth.speed < 127) {
      sequencer.speed_mod = Sequencer::HalfSpeed;
    }
  } else if (double_speed) {
    sequencer.speed_mod = Sequencer::DoubleSpeed;
  } else {
    sequencer.speed_mod = Sequencer::NormalSpeed;
  }
  if (sequencer.tick_clock()) {
    if (random_flag) {
      sequencer.step_offset = random(1, (Sequencer::NUM_STEPS - 2));
    } else {
      sequencer.step_offset = 0;
    }
  }
}

static void sequencer_randomize_step_offset() {
  sequencer.step_offset = random(1, (Sequencer::NUM_STEPS - 2));
  if (!sequencer.is_running()) {
    sequencer.advance();
  }
}

static void sequencer_align_clock() { sequencer.align_clock(); }

static void sequencer_init() {
  last_sequencer_update = millis();
  for (int i = 0; i < Sequencer::NUM_STEPS; i++) {
    sequencer.set_step_note(i, SCALE[random(9)]);
  }

  tempo_handler.setHandleTempoEvent(sequencer_tick_clock);
  tempo_handler.setHandleAlignEvent(sequencer_align_clock);
  tempo_handler.setPPQN(Sequencer::PULSES_PER_QUARTER_NOTE);
  sequencer_stop();
  double_speed = false;
}

static void sequencer_restart() {
  MIDI.sendRealTime(midi::Start);
  delay(1);
  reset_midi_clock();
  sequencer.restart();
}

#define keyboard_set_note(note) sequencer.hold_note(note)
#define keyboard_unset_note(note) sequencer.release_note(note)

#endif
