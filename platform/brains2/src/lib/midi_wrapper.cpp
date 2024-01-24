#include "midi_wrapper.h"
#include "Arduino.h" // For HardwareSerial
#include <MIDI.h>
#include <USB-MIDI.h>

static USBMIDI_NAMESPACE::usbMidiTransport usbTransport(0);
static MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>
    usb_midi(usbTransport);

static MIDI_NAMESPACE::SerialMIDI<HardwareSerial> serialTransport(Serial);
static MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>>
    serial_midi(serialTransport);

#define ALL_TRANSPORTS(function_call)                                          \
  serial_midi.function_call;                                                   \
  usb_midi.function_call;

void MIDI::init(const byte channel, const Callbacks &callbacks) {
  ALL_TRANSPORTS(begin());
  ALL_TRANSPORTS(setHandleClock(callbacks.clock));
  ALL_TRANSPORTS(setHandleNoteOn(callbacks.note_on));
  ALL_TRANSPORTS(setHandleNoteOff(callbacks.note_off));
  ALL_TRANSPORTS(setHandleNoteOff(callbacks.note_off));
  ALL_TRANSPORTS(setHandleStart(callbacks.start));
  ALL_TRANSPORTS(setHandleStop(callbacks.stop));
  ALL_TRANSPORTS(setHandleContinue(callbacks.cont));
  ALL_TRANSPORTS(setHandleControlChange(callbacks.cc));
  ALL_TRANSPORTS(setHandleSystemExclusive(callbacks.sysex));
}

void MIDI::read(const byte channel) {
  ALL_TRANSPORTS(read(channel));
}

void MIDI::sendRealTime(const midi::MidiType message) {
  ALL_TRANSPORTS(sendRealTime(message));
}

void MIDI::sendControlChange(const byte cc, const byte value,
                             const byte channel) {
  ALL_TRANSPORTS(sendControlChange(cc, value, channel));
}

void MIDI::sendNoteOn(const byte note, const byte velocity,
                      const byte channel) {
  ALL_TRANSPORTS(sendNoteOn(note, velocity, channel));
}

void MIDI::sendNoteOff(const byte note, const byte velocity,
                       const byte channel) {
  ALL_TRANSPORTS(sendNoteOff(note, velocity, channel));
}

void MIDI::sendSysEx(const unsigned length, const byte *bytes) {
  ALL_TRANSPORTS(sendSysEx(length, bytes));
}
