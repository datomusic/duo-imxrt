#include "sequencer_helpers.h"

#define SINGLE_TEST stops_playing_note_after_gate_duration

namespace NoteTracker {
static bool note_active = false;
static int activation_count = 0;
static void reset() {
  note_active = false;
  activation_count = 0;
}
static void note_on(uint8_t note, uint8_t velocity) {
  TEST_ASSERT_MESSAGE(note_active == false, "note_on called twice");
  printf("Note on: %i\n", note);
  activation_count++;
  note_active = true;
}
static void note_off(void) {
  TEST_ASSERT_MESSAGE(note_active == true, "note_off called twice");
  printf("Note off\n");
  note_active = false;
}
static Sequencer::Callbacks callbacks{note_on, note_off};
} // namespace NoteTracker

namespace Tests {

void holds_note_if_not_running() {
  Sequencer seq(NoteTracker::callbacks);
  clear_steps(seq);
  ASSERT_EQ(seq.is_running(), false);

  seq.hold_note(1);
  const uint8_t note_len = 10;
  seq.update_notes(1, note_len, 0);
  ASSERT_EQ(1, NoteTracker::activation_count);
  ASSERT_EQ(true, NoteTracker::note_active);
  const uint32_t long_delta = 1000000;
  seq.update_notes(long_delta, note_len, 0);

  // Note should still be on after note_len has passed.
  ASSERT_EQ(true, NoteTracker::note_active);
  ASSERT_EQ(1, NoteTracker::activation_count);

  seq.release_note(1);
  seq.update_notes(0, long_delta, note_len);
  ASSERT_EQ(false, NoteTracker::note_active);
}

void stops_playing_note_after_gate_duration() {
  Sequencer seq(NoteTracker::callbacks);
  clear_steps(seq);

  seq.start();
  seq.hold_note(0);
  ASSERT_EQ(false, NoteTracker::note_active);
  seq.advance(0);
  seq.release_note(0);
  ASSERT_EQ(true, NoteTracker::note_active);
  ASSERT_EQ(1, NoteTracker::activation_count);

  const auto note_len = 10;
  seq.update_notes(note_len - 1, note_len, 0);
  ASSERT_EQ(true, NoteTracker::note_active);

  seq.update_notes(1, note_len, 0);
  ASSERT_EQ(1, NoteTracker::activation_count);
  ASSERT_EQ(NoteTracker::note_active, false);
}

void does_not_replay_active_note() {
  Sequencer seq(NoteTracker::callbacks);
  clear_steps(seq);

  // TODO:
  // Test that trigger_note does not call note_on for the same
  // held note unless note_off was called.
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
  RUN_TEST(Tests::does_not_replay_active_note);
#endif
  return UNITY_END();
}
