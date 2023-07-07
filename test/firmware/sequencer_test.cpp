#include "sequencer_helpers.h"

// #define SINGLE_TEST respects_step_offset_during_playback

namespace NoteTracker {
static bool note_active = false;
static int activation_count = 0;
static int last_note = -1;

static void reset() {
  note_active = false;
  activation_count = 0;
}
static void note_on(uint8_t note, uint8_t velocity) {
  TEST_ASSERT_MESSAGE(note_active == false, "note_on called twice.");
  // printf("Note on: %i\n", note);
  activation_count++;
  last_note = note;
  note_active = true;
}
static void note_off(void) {
  TEST_ASSERT_MESSAGE(note_active == true, "note_off called twice.");
  // printf("Note off\n");
  note_active = false;
}
static Sequencer::Callbacks callbacks{note_on, note_off};

} // namespace NoteTracker

#define ASSERT_NOTE_ACTIVE(active)                                             \
  if (active) {                                                                \
    TEST_ASSERT_TRUE_MESSAGE(NoteTracker::note_active,                         \
                             "Expected active note.");                         \
  } else {                                                                     \
    TEST_ASSERT_FALSE_MESSAGE(NoteTracker::note_active,                        \
                              "Expected inactive note.");                      \
  }

#define ASSERT_ACTIVATED_NOTE_COUNT(count)                                     \
  TEST_ASSERT_EQUAL_MESSAGE(count, NoteTracker::activation_count,              \
                            "Expected activated note count.");

Sequencer make_cleared_sequencer() {
  Sequencer seq(NoteTracker::callbacks);
  clear_steps(seq);
  return seq;
}

namespace Tests {

void holds_note_if_not_running() {
  auto seq = make_cleared_sequencer();
  seq.gate_length_msec = 10;

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
  auto seq = make_cleared_sequencer();
  seq.gate_length_msec = 10;

  seq.start();
  seq.hold_note(0);
  ASSERT_EQ(false, NoteTracker::note_active);
  tick_to_next_step(seq);
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
  auto seq = make_cleared_sequencer();
  seq.gate_length_msec = 10;
  seq.start();

  const auto note = 1;
  seq.hold_note(note);
  seq.update_notes(1);
  seq.release_note(note);
  seq.update_notes(1);

  ASSERT_ONLY_ENABLED_STEP(seq, 0);
  ASSERT_NOTE_ACTIVE(true);
  ASSERT_ACTIVATED_NOTE_COUNT(1);

  tick_to_next_step(seq);
  ASSERT_ONLY_ENABLED_STEP(seq, 0);
}
void records_late_live_note() {
  auto seq = make_cleared_sequencer();
  seq.gate_length_msec = 10;
  seq.start();

  ASSERT_EQ(0, seq.get_cur_step());
  for (unsigned i = 0; i < Sequencer::TICKS_PER_STEP - 1; ++i) {
    seq.tick_clock();
  }

  ASSERT_EQ(0, seq.get_cur_step());

  const auto note = 1;
  seq.hold_note(note);
  seq.update_notes(1);
  seq.release_note(note);
  seq.update_notes(1);

  ASSERT_ONLY_ENABLED_STEP(seq, 1);
  ASSERT_NOTE_ACTIVE(true);
  ASSERT_ACTIVATED_NOTE_COUNT(1);

  tick_to_next_step(seq);
  ASSERT_ONLY_ENABLED_STEP(seq, 1);
}

void records_step_and_advances_when_not_running() {
  auto seq = make_cleared_sequencer();
  ASSERT_FALSE(seq.is_running());
  seq.hold_note(1);
  seq.update_notes(1);
  seq.release_note(1);
  seq.update_notes(1);

  ASSERT_ONLY_ENABLED_STEP(seq, 0);
  ASSERT_EQ(1, seq.get_cur_step());
}

void retriggers_held_notes_on_advance() {
  auto seq = make_cleared_sequencer();
  seq.start();
  seq.gate_length_msec = 2;

  seq.hold_note(1);
  seq.update_notes(1);

  ASSERT_NOTE_ACTIVE(true);
  ASSERT_ACTIVATED_NOTE_COUNT(1);

  tick_to_next_step(seq);

  ASSERT_NOTE_ACTIVE(true);
  ASSERT_ACTIVATED_NOTE_COUNT(2);

  seq.update_notes(seq.gate_length_msec);

  ASSERT_ACTIVATED_NOTE_COUNT(2);
  ASSERT_NOTE_ACTIVE(false);
}

void respects_step_offset_during_playback() {
  auto seq = make_cleared_sequencer();
  for (int i = 0; i < Sequencer::NUM_STEPS; ++i) {
    seq.set_step_note(i, i);
  }
  set_all_steps_active(seq, true);

  seq.start();
  ASSERT_EQ(0, NoteTracker::activation_count);
  tick_to_next_step(seq);
  ASSERT_EQ(1, NoteTracker::last_note);
  ASSERT_EQ(1, NoteTracker::activation_count);
  seq.step_offset = 2;
  tick_to_next_step(seq);
  ASSERT_EQ(2, NoteTracker::activation_count);
  ASSERT_EQ(4, NoteTracker::last_note);
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
  RUN_TEST(Tests::retriggers_held_notes_on_advance);
  RUN_TEST(Tests::respects_step_offset_during_playback);
#endif
  return UNITY_END();
}
