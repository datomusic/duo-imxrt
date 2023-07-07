#include "sequencer_helpers.h"

// #define SINGLE_TEST stops_playing_note_after_gate_duration

namespace NoteTracker {
static bool note_active = false;
static int played_notes = 0;
static int last_note = -1;

static void reset() {
  note_active = false;
  played_notes = 0;
}
static void note_on(uint8_t note, uint8_t velocity) {
  TEST_ASSERT_MESSAGE(note_active == false, "note_on called twice.");
  played_notes++;
  last_note = note;
  note_active = true;
}
static void note_off(void) {
  TEST_ASSERT_MESSAGE(note_active == true, "note_off called twice.");
  note_active = false;
}
static Sequencer::Callbacks callbacks{note_on, note_off};

} // namespace NoteTracker

#define ASSERT_NOTE_PLAYING(active)                                            \
  if (active) {                                                                \
    TEST_ASSERT_TRUE_MESSAGE(NoteTracker::note_active,                         \
                             "Expected playing note.");                        \
  } else {                                                                     \
    TEST_ASSERT_FALSE_MESSAGE(NoteTracker::note_active,                        \
                              "Expected no playing notes.");                   \
  }

#define ASSERT_PLAYED_COUNT(count)                                             \
  TEST_ASSERT_EQUAL_MESSAGE(count, NoteTracker::played_notes,                  \
                            "[Played note count]");

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
  ASSERT_EQ(1, NoteTracker::played_notes);
  ASSERT_TRUE(NoteTracker::note_active);
  const uint32_t long_delta = 1000000;
  seq.update_notes(long_delta);

  // Note should still be on after gate length has passed.
  ASSERT_TRUE(NoteTracker::note_active);
  ASSERT_EQ(1, NoteTracker::played_notes);

  seq.release_note(1);
  seq.update_notes(long_delta);
  ASSERT_EQ(false, NoteTracker::note_active);
}

void stops_playing_note_after_gate_duration() {
  auto seq = make_cleared_sequencer();
  seq.gate_length_msec = 10;
  seq.start();
  const auto note = 123;
  seq.set_step_note(1, note);
  seq.toggle_step(1);

  ASSERT_PLAYED_COUNT(0);
  ASSERT_NOTE_PLAYING(false)

  tick_to_next_step(seq);
  ASSERT_EQ(1, seq.get_cur_step());
  seq.update_notes(1);

  ASSERT_NOTE_PLAYING(true);
  seq.update_notes(seq.gate_length_msec - 1);
  ASSERT_NOTE_PLAYING(true);

  seq.update_notes(1);
  ASSERT_PLAYED_COUNT(1);
  ASSERT_NOTE_PLAYING(false);
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
  ASSERT_NOTE_PLAYING(true);
  ASSERT_PLAYED_COUNT(1);

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
  ASSERT_NOTE_PLAYING(true);
  ASSERT_PLAYED_COUNT(1);

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

  seq.hold_note(1);
  seq.update_notes(1);

  ASSERT_NOTE_PLAYING(true);
  ASSERT_PLAYED_COUNT(1);

  tick_to_next_step(seq);

  ASSERT_NOTE_PLAYING(true);
  ASSERT_PLAYED_COUNT(2);

  seq.update_notes(seq.gate_length_msec);

  ASSERT_PLAYED_COUNT(2);
  ASSERT_NOTE_PLAYING(false);
}

void respects_step_offset_during_playback() {
  auto seq = make_cleared_sequencer();
  for (int i = 0; i < Sequencer::NUM_STEPS; ++i) {
    seq.set_step_note(i, i);
  }

  set_all_steps_active(seq, true);
  ASSERT_EQ(Sequencer::NUM_STEPS, count_enabled_steps(seq));

  seq.start();
  ASSERT_PLAYED_COUNT(0);

  seq.advance();
  seq.update_notes(1);

  ASSERT_PLAYED_COUNT(1);
  ASSERT_EQ(1, NoteTracker::last_note);
  seq.step_offset = 2;

  seq.advance();
  seq.update_notes(1);
  ASSERT_EQ(2, NoteTracker::played_notes);
  ASSERT_EQ(4, NoteTracker::last_note);
  ASSERT_EQ(4, seq.get_cur_step());
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
