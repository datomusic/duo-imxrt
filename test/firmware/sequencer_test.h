#include "firmware/seq.h"
#include "unity.h"

#define ASSERT_ELEMS_UINT8 TEST_ASSERT_EACH_EQUAL_UINT8
#define ASSERT TEST_ASSERT
#define ASSERT_EQ TEST_ASSERT_EQUAL

#define each_step(seq, body)                                                  \
  for (int i = 0; i < SEQUENCER_NUM_STEPS; ++i) {                              \
    const uint8_t note = seq.step_note[i];                                     \
    const uint8_t step_enabled = seq.step_enable[i];                           \
    body                                                                      \
  }

uint8_t get_step_enabled(Sequencer &seq, int index) {
  TEST_ASSERT_LESS_THAN_INT(SEQUENCER_NUM_STEPS, index);
  return seq.step_enable[index];
}

uint8_t get_note(Sequencer &seq, int index) {
  TEST_ASSERT_LESS_THAN_INT(SEQUENCER_NUM_STEPS, index);
  return seq.step_note[index];
}

void clear_steps(Sequencer &s) {
  for (int i = 0; i < SEQUENCER_NUM_STEPS; ++i) {
    s.step_note[i] = s.step_enable[i] = 0;
  }
}

int count_enabled_steps(Sequencer &seq) {
  int count = 0;
  each_step(seq, {
    if (step_enabled) {
      count++;
    }
  });
  return count;
}

void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {}
void note_off(void) {}

void test_Seq_records_note() {
  Sequencer seq(Sequencer::Callbacks{note_on, note_off});

  clear_steps(seq);
  ASSERT_EQ(count_enabled_steps(seq), 0);
  seq.activate_note(1);
  const int gate_len = 100;
  const int offset = 0;
  uint32_t millis = 0;

  seq.keyboard_to_note(millis, offset);
  ASSERT_EQ(count_enabled_steps(seq), 1);
  seq.deactivate_note(1);
  seq.keyboard_to_note(millis, offset);
  ASSERT_EQ(count_enabled_steps(seq), 1);
}

void Sequencer_run_tests() { RUN_TEST(test_Seq_records_note); }
