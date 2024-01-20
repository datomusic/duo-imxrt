#include "midi.h"

static USBMIDI_NAMESPACE::usbMidiTransport usbTransport(0);
static MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>
    usb(usbTransport);

static MIDI_NAMESPACE::SerialMIDI<HardwareSerial> serialTransport(Serial);
static MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>>
    serial(serialTransport);

#define BOTH(function_call)                                                    \
  serial.function_call;                                                        \
  usb.function_call;

void MIDI_IO::init(const byte channel) {
  BOTH(begin());
}

void MIDI_IO::handle(const byte channel) {
  BOTH(read(channel));
}

void MIDI_IO::setHandleNoteOn(NoteCallback callback) {
  BOTH(setHandleNoteOn(callback));
}
void MIDI_IO::setHandleNoteOff(NoteCallback callback) {
  BOTH(setHandleNoteOff(callback));
}

void MIDI_IO::setHandleControlChange(NoteCallback callback) {
  BOTH(setHandleControlChange(callback));
}

void MIDI_IO::setHandleStart(VoidCallback callback) {
  BOTH(setHandleStart(callback));
}

void MIDI_IO::setHandleStop(VoidCallback callback) {
  BOTH(setHandleStop(callback));
}

void MIDI_IO::setHandleContinue(VoidCallback callback) {
  BOTH(setHandleContinue(callback));
}

void MIDI_IO::setHandleClock(VoidCallback callback) {
  BOTH(setHandleClock(callback));
}

void MIDI_IO::setHandleSystemExclusive(SyxCallback callback) {
  BOTH(setHandleSystemExclusive(callback));
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
