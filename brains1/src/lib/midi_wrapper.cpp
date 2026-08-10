#include "midi_wrapper.h"
#include "Arduino.h" // For HardwareSerial and usbMIDI
#include <MIDI.h>

// DIN MIDI is on Serial1 (pins 0/1)
static MIDI_NAMESPACE::MidiInterface<HardwareSerial> serial_midi(Serial1);

// The Teensy 3 core usbMIDI has no sysex or start/stop/continue handlers,
// so those are dispatched by hand in read() and from the realtime callback.
static MIDI::Callbacks usb_callbacks = {};

static void usb_handle_realtime(uint8_t type) {
  switch (type) {
    case 0xF8: // Clock
      if (usb_callbacks.clock) usb_callbacks.clock();
      break;
    case 0xFA: // Start
      if (usb_callbacks.start) usb_callbacks.start();
      break;
    case 0xFB: // Continue
      if (usb_callbacks.cont) usb_callbacks.cont();
      break;
    case 0xFC: // Stop
      if (usb_callbacks.stop) usb_callbacks.stop();
      break;
  }
}

void MIDI::init(const Callbacks &callbacks) {
  usb_callbacks = callbacks;

  serial_midi.begin();
  serial_midi.setHandleClock(callbacks.clock);
  serial_midi.setHandleNoteOn(callbacks.note_on);
  serial_midi.setHandleNoteOff(callbacks.note_off);
  serial_midi.setHandleStart(callbacks.start);
  serial_midi.setHandleStop(callbacks.stop);
  serial_midi.setHandleContinue(callbacks.cont);
  serial_midi.setHandleControlChange(callbacks.cc);
  serial_midi.setHandleSystemExclusive(callbacks.sysex);

  usbMIDI.setHandleNoteOn(callbacks.note_on);
  usbMIDI.setHandleNoteOff(callbacks.note_off);
  usbMIDI.setHandleControlChange(callbacks.cc);
  usbMIDI.setHandleRealTimeSystem(usb_handle_realtime);
}

void MIDI::read(const byte channel) {
  serial_midi.read(channel);
  while (usbMIDI.read(channel)) {
    // Sysex (type 7) has no handler in the old core; length is in data1
    if (usbMIDI.getType() == 7 && usb_callbacks.sysex) {
      usb_callbacks.sysex(usbMIDI.getSysExArray(), usbMIDI.getData1());
    }
  }
}

void MIDI::sendRealTime(const midi::MidiType message) {
  serial_midi.sendRealTime(message);
  usbMIDI.sendRealTime(message);
}

void MIDI::sendControlChange(const byte cc, const byte value,
                             const byte channel) {
  serial_midi.sendControlChange(cc, value, channel);
  usbMIDI.sendControlChange(cc, value, channel);
}

void MIDI::sendNoteOn(const byte note, const byte velocity,
                      const byte channel) {
  serial_midi.sendNoteOn(note, velocity, channel);
  usbMIDI.sendNoteOn(note, velocity, channel);
}

void MIDI::sendNoteOff(const byte note, const byte velocity,
                       const byte channel) {
  serial_midi.sendNoteOff(note, velocity, channel);
  usbMIDI.sendNoteOff(note, velocity, channel);
}

void MIDI::sendSysEx(const unsigned length, const byte *bytes) {
  serial_midi.sendSysEx(length, bytes);
  usbMIDI.sendSysEx(length, bytes);
}
