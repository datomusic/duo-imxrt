#ifndef Sequencer_h
#define Sequencer_h
/*
 The sequencer holds a number of notes
 Note timing is divided into 24 steps per quarter note
 */

#define PULSES_PER_QUARTER_NOTE 24
#define PULSES_PER_EIGHT_NOTE   PULSES_PER_QUARTER_NOTE / 2
const uint8_t SEQUENCER_NUM_STEPS = 8;


//Initial sequencer values
uint8_t step_note[] = { 1,0,6,9,0,4,0,5 };
uint8_t step_enable[] = { 1,0,1,1,1,1,0,1 };

void sequencer_init();
void sequencer_restart();
void sequencer_start();
void sequencer_stop();
void sequencer_advance();
void sequencer_tick_clock();
void sequencer_reset();
void sequencer_update();
void keyboard_to_note();
int keyboard_get_highest_note();
int keyboard_get_latest_note();
void sequencer_align_clock();

namespace Sequencer{
  static void trigger_current_note();
  static void untrigger_note();
  static void advance_without_play();

  void record_note(const uint8_t note){
    step_note[current_step] = note;
    step_enable[current_step] = true;
  }
}

bool sequencer_is_running = false;

uint32_t next_step_time = 0;
uint32_t gate_off_time = 0;
uint32_t note_on_time;
uint32_t previous_note_on_time;
uint32_t note_off_time;

static bool double_speed = false;

enum NoteState{
  Idle,
  Playing
};

NoteState note_state = Idle;

void sequencer_init() {
  note_stack.Init();

  for(int i = 0; i < SEQUENCER_NUM_STEPS; i++) {
    step_note[i] = SCALE[random(9)];
  }
  tempo_handler.setHandleTempoEvent(sequencer_tick_clock);
  tempo_handler.setHandleAlignEvent(sequencer_align_clock);
  tempo_handler.setPPQN(PULSES_PER_QUARTER_NOTE);
  sequencer_stop();
  current_step = SEQUENCER_NUM_STEPS - 1;
  double_speed = false;
}

static void reset_midi_clock(){
  midi_clock = 0;
  tempo_handler.midi_clock_reset();
}

void sequencer_restart() {
  MIDI.sendRealTime(midi::Start);
  delay(1);
  current_step = SEQUENCER_NUM_STEPS - 1;
  reset_midi_clock();
  sequencer_is_running = true;
  sequencer_clock = 0;
}

void sequencer_align_clock() {
  //round sequencer_clock to the nearest 12
  if(sequencer_clock%12 > 6) {
    sequencer_clock += 12-(sequencer_clock%12);
  } else {
    sequencer_clock -= (sequencer_clock%12);
  }
}

void sequencer_reset_clock() {
  sequencer_clock = 0;
}

void sequencer_start() {
  MIDI.sendRealTime(midi::Continue);
  usbMIDI.sendRealTime(midi::Continue);
  reset_midi_clock();
  sequencer_is_running = true;
}

void sequencer_stop() {
  if(sequencer_is_running) {

    usbMIDI.sendControlChange(123,0,MIDI_CHANNEL);
    MIDI.sendControlChange(123,0,MIDI_CHANNEL);
    usbMIDI.sendRealTime(midi::Stop);
    MIDI.sendRealTime(midi::Stop);

    sequencer_is_running = false;
    Sequencer::untrigger_note();
  }
  midi_clock = 0;
}

void sequencer_toggle_start() {
  if(sequencer_is_running) {
    sequencer_stop();
  } else {
    sequencer_start();
  }
}

void sequencer_tick_clock() {
  uint8_t sequencer_divider = PULSES_PER_EIGHT_NOTE;
  if(double_speed) {
    sequencer_divider = PULSES_PER_EIGHT_NOTE / 2;
  }

  if(!tempo_handler.is_clock_source_internal()) {
    int potvalue = synth.speed;
    if(potvalue > 900) {
      sequencer_divider /= 2;
    } else if(potvalue < 127) {
      sequencer_divider *= 2;
    }
  }

  if(sequencer_is_running && (sequencer_clock % sequencer_divider)==0) {
    sequencer_advance();
  } 
  sequencer_clock++;
}


void Sequencer::advance_without_play() {
  static uint8_t arpeggio_index = 0;

  if(note_state == Playing) {
    Sequencer::untrigger_note();
  }
  current_step++;
  current_step%=SEQUENCER_NUM_STEPS;

  if (!next_step_is_random && !random_flag) {
    random_offset = 0;
  } else {
    random_flag = false;
    random_offset = random(1,(SEQUENCER_NUM_STEPS - 2));
    //current_step = ((current_step + random(2, SEQUENCER_NUM_STEPS))%SEQUENCER_NUM_STEPS);
  }

  // Sample keys
  const uint8_t note_count = note_stack.size();

  if(arpeggio_index >= note_count) {
    arpeggio_index = 0;
  }

  if(note_count > 0) {
    if(!sequencer_is_running) {
      Sequencer::record_note(note_stack.most_recent_note().note);
    } else {
      Sequencer::record_note(note_stack.sorted_note(arpeggio_index).note);
      arpeggio_index++;
    }
  }
}

void sequencer_advance() {
  Sequencer::advance_without_play();
  Sequencer::trigger_current_note();
}

void sequencer_reset() {
  current_step = SEQUENCER_NUM_STEPS;
}

void sequencer_update() {
  gate_length_msec = map(synth.gateLength,0,1023,10,200);
  tempo_handler.update(midi_clock);

  if(note_state == Playing && millis() >= note_off_time) { 
    Sequencer::untrigger_note();
  }
}

static void Sequencer::trigger_current_note() {
  note_state = Playing;
  previous_note_on_time = millis();
  note_off_time = previous_note_on_time + gate_length_msec;


  note_on(step_note[((current_step+random_offset)%SEQUENCER_NUM_STEPS)]+transpose,
      INITIAL_VELOCITY,
      step_enable[((current_step+random_offset)%SEQUENCER_NUM_STEPS)]);
}

static void Sequencer::untrigger_note() {
  note_state = Idle;
  note_off();
  note_off_time = millis() + tempo_interval - gate_length_msec; // Set note off time to sometime in the future
}


void keyboard_set_note(uint8_t note) {
  note_stack.NoteOn(note, INITIAL_VELOCITY);
}

void keyboard_unset_note(uint8_t note) {
  note_stack.NoteOff(note);
}

void keyboard_to_note() {
  static uint8_t last_note = 255;
  static uint8_t last_stack_size = 255;

  const uint8_t recent_note = note_stack.most_recent_note().note;

  const auto stack_size = note_stack.size();
  if (stack_size != last_stack_size) {
    last_stack_size = stack_size;
    if (stack_size > 0) {
      if (recent_note != last_note) {
        if (!sequencer_is_running) {
          Sequencer::advance_without_play();
        }

        Sequencer::record_note(recent_note);
        Sequencer::trigger_current_note();
        last_note = recent_note;
      }
    } else {
      note_off();
      last_note = 255; // Make sure this is a non existing note in the scale
    }
  }
}

#endif
