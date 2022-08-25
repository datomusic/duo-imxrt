#include "midi_stub.h"

int MIDI_CHANNEL = 1;

// Musical settings
const uint8_t SCALE[] = { 49,51,54,56,58,61,63,66,68,70 };
const uint8_t SCALE_OFFSET_FROM_C3[] { 1,3,6,8,10,13,15,18,20,22 };

#define HIGH_SAMPLE_RATE 44100
#define LOW_SAMPLE_RATE 2489

#define INITIAL_VELOCITY 100

#define EEPROM_MIDI_CHANNEL 0

// Globals that should not be globals
int gate_length_msec = 40;

uint32_t sequencer_clock = 0;
// Sequencer settings
uint8_t current_step;
int tempo = 0;
uint8_t set_key = 9;
float osc_saw_frequency = 0.;
float osc_pulse_frequency = 0.;
float osc_pulse_target_frequency = 0.;
float osc_saw_target_frequency = 0.;
uint8_t osc_pulse_midi_note = 0;
uint8_t note_is_playing = 0;
bool note_is_triggered = false;
int transpose = 0;
bool next_step_is_random = false;
int tempo_interval;
bool random_flag = 0;
bool dfu_flag = 0;
bool in_setup = true;

int random_offset = 0;
uint32_t midi_clock = 0;
uint16_t audio_peak_values = 0UL;

void keys_scan();
void pots_read();
void drum_init();
void drum_read();
void print_log();

void note_on(uint8_t midi_note, uint8_t velocity, bool enabled);
void note_off();

void keyboard_to_note();
float detune(int note, int amount);

int tempo_interval_msec();

void enter_dfu();

// 10 bit representation of full scale parameters
typedef struct {
  int detune;
  int pulseWidth;
  int filter;
  int resonance;
  int release;
  int amplitude;
  bool glide;
  bool accent;
  bool delay;
  bool crush;
} synth_parameters;

synth_parameters synth;

void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {}
void note_off() {}

int potRead(uint8_t num) {}

const int SYNC_DETECT = 0;
const int SYNC_OUT_PIN = 0;
const int SYNC_IN = 0;
const int TEMPO_POT = 0;

#include "../note_stack.h"
NoteStack<10> note_stack;

typedef int elapsedMillis;
#include "../TempoHandler.h"
TempoHandler tempo_handler;





