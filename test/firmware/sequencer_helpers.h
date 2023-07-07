#ifndef SEQUENCER_HELPERS_H_VRO3THZL
#define SEQUENCER_HELPERS_H_VRO3THZL

#include "firmware/seq.h"
#include "unity.h"

#define ASSERT_ELEMS_UINT8 TEST_ASSERT_EACH_EQUAL_UINT8
#define ASSERT TEST_ASSERT
#define ASSERT_EQ TEST_ASSERT_EQUAL
#define ASSERT_TRUE TEST_ASSERT_TRUE
#define ASSERT_FALSE TEST_ASSERT_FALSE

#define ASSERT_ONLY_ENABLED_STEP(seq, step_index)                              \
  TEST_ASSERT_EQUAL_MESSAGE(1, count_enabled_steps(seq),                       \
                            "Expected single enabled step.");                  \
  TEST_ASSERT_TRUE_MESSAGE(get_step_enabled(seq, step_index),                  \
                           "Expected step to be active: " #step_index);

uint8_t get_step_enabled(const Sequencer &seq, int index) {
  TEST_ASSERT_LESS_THAN_INT(Sequencer::NUM_STEPS, index);
  return seq.get_step_enabled(index);
}

uint8_t get_note(Sequencer &seq, int index) {
  TEST_ASSERT_LESS_THAN_INT(Sequencer::NUM_STEPS, index);
  return seq.get_step_note(index);
}

void set_all_steps_active(Sequencer &s, const bool active) {
  for (int i = 0; i < Sequencer::NUM_STEPS; ++i) {
    if (s.get_step_enabled(i) != active) {
      s.toggle_step(i);
    }
  }
}

void clear_steps(Sequencer &s) { set_all_steps_active(s, false); }

int count_enabled_steps(const Sequencer &seq) {
  int count = 0;
  for (int i = 0; i < Sequencer::NUM_STEPS; ++i) {
    if (seq.get_step_enabled(i)) {
      count++;
    }
  }

  return count;
}

void tick_to_next_step(Sequencer &seq) {
  const auto last_step = seq.get_cur_step();
  while (last_step == seq.get_cur_step()) {
    seq.tick_clock();
  }

  ASSERT_EQ(0, seq.get_clock() % Sequencer::TICKS_PER_STEP);
}

#endif /* end of include guard: SEQUENCER_HELPERS_H_VRO3THZL */
