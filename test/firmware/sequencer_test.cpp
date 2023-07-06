#include "sequencer_helpers.h"

namespace NoteTracker {
static bool note_active = false;
static int activation_count = 0;
static void reset() {
  note_active = false;
  activation_count = 0;
}
static void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {
  activation_count++;
  note_active = true;
}
static void note_off(void) { note_active = false; }
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
  ASSERT_EQ(NoteTracker::activation_count, 1);
  ASSERT_EQ(NoteTracker::note_active, true);
  const uint32_t long_delta = 1000000;
  seq.update_notes(long_delta, note_len, 0);
  ASSERT_EQ(NoteTracker::activation_count, 1);
  // Note should still be on after note_len has passed.
  ASSERT_EQ(NoteTracker::note_active, true);
  seq.release_note(1);
  seq.update_notes(long_delta, note_len, 0);
  ASSERT_EQ(NoteTracker::note_active, false);
}

void stops_playing_note_after_gate_duration() {
  Sequencer seq(NoteTracker::callbacks);
  clear_steps(seq);

  // TODO:
  // note_off should be called correctly after enough calls to update()
  // with an accumulated delta past the note gate length.
} // namespace stops_playing_note_after_gate_duration

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
  RUN_TEST(Tests::stops_playing_note_after_gate_duration);
  RUN_TEST(Tests::does_not_replay_active_note);
  RUN_TEST(Tests::holds_note_if_not_running);
  return UNITY_END();
}
