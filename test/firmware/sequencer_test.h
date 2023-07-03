#include "firmware/seq.h"
#include "unity.h"

void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {}
void note_off(void) {}

// TODO: Dummy test
void test_Seq_start() {
  Sequencer seq(Sequencer::Callbacks{note_on, note_off});
  seq.start();
}

void Sequencer_run_tests() { RUN_TEST(test_Seq_start); }
