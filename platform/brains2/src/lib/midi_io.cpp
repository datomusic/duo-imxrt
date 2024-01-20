#include "midi_io.h"
#include "Arduino.h" // For HardwareSerial
#include <USB-MIDI.h>

static USBMIDI_NAMESPACE::usbMidiTransport usbTransport(0);
static MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>
    usb(usbTransport);

static MIDI_NAMESPACE::SerialMIDI<HardwareSerial> serialTransport(Serial);
static MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>>
    serial(serialTransport);

#define BOTH(function_call)                                                    \
  serial.function_call;                                                        \
  usb.function_call;

void MIDI_IO::init(const byte channel, const Callbacks &callbacks) {
  BOTH(begin());
  BOTH(setHandleClock(callbacks.clock));
  BOTH(setHandleNoteOn(callbacks.note_on));
  BOTH(setHandleNoteOff(callbacks.note_off));
  BOTH(setHandleNoteOff(callbacks.note_off));
  BOTH(setHandleStart(callbacks.start));
  BOTH(setHandleStop(callbacks.stop));
  BOTH(setHandleContinue(callbacks.cont));
  BOTH(setHandleControlChange(callbacks.cc));
  BOTH(setHandleSystemExclusive(callbacks.sysex));
}

void MIDI_IO::read(const byte channel) {
  BOTH(read(channel));
}

void MIDI_IO::sendRealtime(const midi::MidiType message) {
  BOTH(sendRealTime(message));
}

void MIDI_IO::sendControlChange(byte cc, byte value, byte channel) {
  BOTH(sendControlChange(cc, value, channel));
}

void MIDI_IO::sendNoteOn(byte inNoteNumber, byte inVelocity, byte inChannel) {
  BOTH(sendNoteOn(inNoteNumber, inVelocity, inChannel));
}

void MIDI_IO::sendNoteOff(byte inNoteNumber, byte inVelocity, byte inChannel) {
  BOTH(sendNoteOff(inNoteNumber, inVelocity, inChannel));
}

void MIDI_IO::sendSysEx(unsigned length, const byte *bytes) {
  BOTH(sendSysEx(length, bytes));
}
