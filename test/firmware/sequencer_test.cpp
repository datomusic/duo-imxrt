#include "sequencer_helpers.h"

namespace NoteTracker {
static bool note_active = false;
static void reset() { note_active = false; }
static void note_on(uint8_t midi_note, uint8_t velocity, bool enabled) {}
static void note_off(void) {}
static Sequencer::Callbacks callbacks{note_on, note_off};
} // namespace NoteTracker

namespace Tests {

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

/*
void test_Seq_records_note() {
  Sequencer seq(Sequencer::Callbacks{note_on, note_off});

  clear_steps(seq);
  ASSERT_EQ(count_enabled_steps(seq), 0);
  seq.hold_note(1);
  const int gate_len = 100;
  const int offset = 0;
  uint32_t millis = 0;

  seq.keyboard_to_note(millis, offset);
  ASSERT_EQ(count_enabled_steps(seq), 1);
  seq.release_note(1);
  seq.keyboard_to_note(millis, offset);
  ASSERT_EQ(count_enabled_steps(seq), 1);
}
*/
} // namespace Tests

void setUp(void) { NoteTracker::reset(); }
void tearDown(void) {}

int main() {
  UNITY_BEGIN();
  RUN_TEST(Tests::stops_playing_note_after_gate_duration);
  RUN_TEST(Tests::does_not_replay_active_note);
  return UNITY_END();
}
