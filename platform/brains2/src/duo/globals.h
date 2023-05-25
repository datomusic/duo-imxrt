#define VERSION "1.2.0-beta1"
const uint8_t FIRMWARE_VERSION[] = { 1, 2, 0 };

// #define DEV_MODE

int MIDI_CHANNEL = 1;

// Musical settings
const uint8_t SCALE[] = { 49,51,54,56,58,61,63,66,68,70 };
const uint8_t SCALE_OFFSET_FROM_C3[] { 1,3,6,8,10,13,15,18,20,22 };

#define HIGH_SAMPLE_RATE 44100.0f
#define LOW_SAMPLE_RATE 2489.0f

#define EEPROM_MIDI_CHANNEL 0

// Globals that should not be globals

int tempo = 0;
float osc_saw_frequency = 0.;
float osc_pulse_frequency = 0.;
float osc_pulse_target_frequency = 0.;
float osc_saw_target_frequency = 0.;
uint8_t osc_pulse_midi_note = 0;
int transpose = 0;
int tempo_interval = 0;
bool dfu_flag = 0;
bool in_setup = true;

uint32_t midi_clock = 0;
uint16_t audio_peak_values = 0UL;

void keys_scan();
void pots_read();
void drum_init();
void drum_read();
void print_log();

uint8_t note_is_playing = 0;
void note_on(uint8_t midi_note, uint8_t velocity, bool enabled);
void note_off();

void keyboard_to_note();
float detune(int note, int amount);

int tempo_interval_msec();

void enter_dfu();

#include "firmware/synth_params.h"
synth_parameters synth;
