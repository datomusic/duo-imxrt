#ifndef SEQUENCER_HELPERS_H_VRO3THZL
#define SEQUENCER_HELPERS_H_VRO3THZL

#include "firmware/seq.h"
#include "unity.h"

#define ASSERT_ELEMS_UINT8 TEST_ASSERT_EACH_EQUAL_UINT8
#define ASSERT TEST_ASSERT
#define ASSERT_EQ TEST_ASSERT_EQUAL
#define ASSERT_TRUE TEST_ASSERT_TRUE
#define ASSERT_FALSE TEST_ASSERT_FALSE

uint8_t get_step_enabled(const Sequencer &seq, int index) {
  TEST_ASSERT_LESS_THAN_INT(Sequencer::NUM_STEPS, index);
  return seq.get_step_enabled(index);
}

uint8_t get_note(Sequencer &seq, int index) {
  TEST_ASSERT_LESS_THAN_INT(Sequencer::NUM_STEPS, index);
  return seq.get_step_note(index);
}

void clear_steps(Sequencer &s) {
  for (int i = 0; i < Sequencer::NUM_STEPS; ++i) {
    if (s.get_step_enabled(i)) {
      s.toggle_step(i);
    }
    s.set_step_note(i, 0);
  }
}

int count_enabled_steps(const Sequencer &seq) {
  int count = 0;
  for (int i = 0; i < Sequencer::NUM_STEPS; ++i) {
    if (seq.get_step_enabled(i)) {
      count++;
    }
  }

  return count;
}

void ASSERT_ONLY_ENABLED_STEP(const Sequencer &seq, const unsigned step_index) {
  ASSERT_TRUE(get_step_enabled(seq, step_index));
  ASSERT_EQ(1, count_enabled_steps(seq));
}

#endif /* end of include guard: SEQUENCER_HELPERS_H_VRO3THZL */
