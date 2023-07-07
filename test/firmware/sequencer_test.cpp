#include "sequencer_helpers.h"

// #define SINGLE_TEST records_late_live_notes

namespace NoteTracker {
static bool note_active = false;
static int activation_count = 0;
static void reset() {
  note_active = false;
  activation_count = 0;
}
static void note_on(uint8_t note, uint8_t velocity) {
  TEST_ASSERT_MESSAGE(note_active == false, "note_on called twice");
  // printf("Note on: %i\n", note);
  activation_count++;
  note_active = true;
}
static void note_off(void) {
  TEST_ASSERT_MESSAGE(note_active == true, "note_off called twice");
  // printf("Note off\n");
  note_active = false;
}
static Sequencer::Callbacks callbacks{note_on, note_off};
} // namespace NoteTracker

namespace Tests {

void holds_note_if_not_running() {
  Sequencer seq(NoteTracker::callbacks);
  seq.gate_length_msec = 10;
  clear_steps(seq);

  ASSERT_EQ(seq.is_running(), false);

  seq.hold_note(1);
  seq.update_notes(1);
  ASSERT_EQ(1, NoteTracker::activation_count);
  ASSERT_TRUE(NoteTracker::note_active);
  const uint32_t long_delta = 1000000;
  seq.update_notes(long_delta);

  // Note should still be on after gate length has passed.
  ASSERT_TRUE(NoteTracker::note_active);
  ASSERT_EQ(1, NoteTracker::activation_count);

  seq.release_note(1);
  seq.update_notes(long_delta);
  ASSERT_EQ(false, NoteTracker::note_active);
}

void stops_playing_note_after_gate_duration() {
  Sequencer seq(NoteTracker::callbacks);
  seq.gate_length_msec = 10;
  clear_steps(seq);

  seq.start();
  seq.hold_note(0);
  ASSERT_EQ(false, NoteTracker::note_active);
  seq.advance();
  seq.release_note(0);
  ASSERT_TRUE(NoteTracker::note_active);
  ASSERT_EQ(1, NoteTracker::activation_count);

  seq.update_notes(seq.gate_length_msec - 1);
  ASSERT_TRUE(NoteTracker::note_active);

  seq.update_notes(1);
  ASSERT_EQ(1, NoteTracker::activation_count);
  ASSERT_EQ(NoteTracker::note_active, false);
}


void records_early_live_note() {
  Sequencer seq(NoteTracker::callbacks);
  clear_steps(seq);
  seq.start();

  seq.hold_note(0);
  seq.update_notes(1);
  seq.release_note(0);
  ASSERT_ONLY_ENABLED_STEP(seq, 0);

  seq.advance();

  ASSERT_ONLY_ENABLED_STEP(seq, 0);
}
void records_late_live_note() {
  Sequencer seq(NoteTracker::callbacks);
  seq.start();
  clear_steps(seq);
  seq.update_notes(0);

  for (unsigned i = 0; i < Sequencer::TICKS_PER_STEP - 1; ++i) {
    seq.inc_clock();
  }

  ASSERT_EQ(0, seq.get_cur_step());

  seq.hold_note(1);
  seq.update_notes(0);
  seq.release_all_notes();
  ASSERT_EQ(1, count_enabled_steps(seq));
  ASSERT_TRUE(get_step_enabled(seq, 1));
}

void records_step_and_advances_when_not_running() {
  Sequencer seq(NoteTracker::callbacks);
  clear_steps(seq);
  ASSERT_FALSE(seq.is_running());
  seq.hold_note(1);
  seq.update_notes(1);
  seq.release_note(1);
  seq.update_notes(1);

  ASSERT_ONLY_ENABLED_STEP(seq, 0);
  ASSERT_EQ(1, seq.get_cur_step());
}
} // namespace Tests

void setUp(void) { NoteTracker::reset(); }
void tearDown(void) {}

int main() {
  UNITY_BEGIN();
#ifdef SINGLE_TEST
  RUN_TEST(Tests::SINGLE_TEST);
#else
  RUN_TEST(Tests::holds_note_if_not_running);
  RUN_TEST(Tests::stops_playing_note_after_gate_duration);
  RUN_TEST(Tests::records_early_live_note);
  RUN_TEST(Tests::records_late_live_note);
  RUN_TEST(Tests::records_step_and_advances_when_not_running);
#endif
  return UNITY_END();
}
