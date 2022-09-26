#include "Arduino.h"
#include "board_init.h"
#include "pin_mux.h"
#include "tusb.h"

/*
void midi_task(void) {
  static uint32_t start_ms = 0;

  uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
  uint8_t const channel = 0;   // 0 for channel 1

  // The MIDI interface always creates input and output port/jack descriptors
  // regardless of these being used or not. Therefore incoming traffic should be
  // read (possibly just discarded) to avoid the sender blocking in IO
  uint8_t packet[4];
  while (tud_midi_available())
    tud_midi_packet_read(packet);

  // send note periodically
  if (board_millis() - start_ms < 286)
    return; // not enough time
  start_ms += 286;

  // Previous positions in the note sequence.
  int previous = (int)(note_pos - 1);

  // If we currently are at position 0, set the
  // previous position to the last note in the sequence.
  if (previous < 0)
    previous = sizeof(note_sequence) - 1;

  // Send Note On for current position at full velocity (127) on channel 1.
  uint8_t note_on[3] = {0x90 | channel, note_sequence[note_pos], 127};
  tud_midi_stream_write(cable_num, note_on, 3);

  // Send Note Off for previous note.
  uint8_t note_off[3] = {0x80 | channel, note_sequence[previous], 0};
  tud_midi_stream_write(cable_num, note_off, 3);

  // Increment position
  note_pos++;

  // If we are at the end of the sequence, start over.
  if (note_pos >= sizeof(note_sequence))
    note_pos = 0;
}
*/

int main(void) {
  board_init();

  tud_init(BOARD_TUD_RHPORT);

  while (true) {
    tud_task(); // tinyusb device task
    // midi_task();
  }
}
