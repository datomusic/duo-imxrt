#include "Arduino.h"
#include "board_init.h"
#include "leds.h"
#include "pin_mux.h"
#include "usb/usb.h"

using LEDs::Pixel;

// Variable that holds the current position in the sequence.
uint32_t note_pos = 0;

// Store example melody as an array of note values
static uint8_t note_sequence[] = {
    74, 78, 81, 86,  90, 93, 98, 102, 57, 61,  66, 69, 73, 78, 81, 85,
    88, 92, 97, 100, 97, 92, 88, 85,  81, 78,  74, 69, 66, 62, 57, 62,
    66, 69, 74, 78,  81, 86, 90, 93,  97, 102, 97, 93, 90, 85, 81, 78,
    73, 68, 64, 61,  56, 61, 64, 68,  74, 78,  81, 86, 90, 93, 98, 102};

static void midi_task(void) {
  static uint32_t start_ms = 0;

  uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
  uint8_t const channel = 0;   // 0 for channel 1

  // send note periodically
  if (millis() - start_ms < 286)
    return; // not enough time
  start_ms += 286;

  // Previous positions in the note sequence.
  int previous = note_pos - 1;

  const uint8_t note_on[3] = {0x90 | channel, note_sequence[note_pos], 127};
  DatoUSB::midi_send(cable_num, note_on, 3);

  // Send Note Off for previous note.
  const uint8_t note_off[3] = {0x80 | channel, note_sequence[previous], 0};
  DatoUSB::midi_send(cable_num, note_off, 3);

  // Increment position
  note_pos++;

  // If we are at the end of the sequence, start over.
  if (note_pos >= sizeof(note_sequence))
    note_pos = 0;
}

int main(void) {
  board_init();
  LEDs::init();
  DatoUSB::init();

  Pixel pixels[1] = {Pixel{0, 0, 0}};
  Pixel &pixel = pixels[0];

  pixel.r = 255;

  while (true) {
    LEDs::show(pixels, 0);

    if (DatoUSB::background_update()) {
      pixel.r = 0;
      pixel.g = 255;
    }

    midi_task();

    delayMicroseconds(10000);
  }
}
