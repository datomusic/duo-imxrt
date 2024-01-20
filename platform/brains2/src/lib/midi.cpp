#include "midi.h"

MIDI_IO::MIDI_IO()
    : usbTransport(0), usb(usbTransport), serialTransport(Serial),
      serial(serialTransport) {
}

void MIDI_IO::init(const byte channel) {
  serial.begin(channel);
  usb.begin(channel);
}

void MIDI_IO::handle(const byte channel) {
  serial.read(channel);
  usb.read(channel);
}

void MIDI_IO::setHandleNoteOn(NoteCallback callback) {
  serial.setHandleNoteOn(callback);
  usb.setHandleNoteOn(callback);
}
void MIDI_IO::setHandleNoteOff(NoteCallback callback) {
  serial.setHandleNoteOff(callback);
  usb.setHandleNoteOff(callback);
}

void MIDI_IO::setHandleControlChange(NoteCallback callback) {
  serial.setHandleControlChange(callback);
  usb.setHandleControlChange(callback);
}

void MIDI_IO::setHandleStart(VoidCallback callback) {
  serial.setHandleStart(callback);
  usb.setHandleStart(callback);
}

void MIDI_IO::setHandleStop(VoidCallback callback) {
  serial.setHandleStop(callback);
  usb.setHandleStop(callback);
}

void MIDI_IO::setHandleContinue(VoidCallback callback) {
  serial.setHandleContinue(callback);
  usb.setHandleContinue(callback);
}

void MIDI_IO::setHandleClock(VoidCallback callback) {
  usb.setHandleClock(callback);
}

void MIDI_IO::setHandleSystemExclusive(SyxCallback callback) {
  usb.setHandleSystemExclusive(callback);
}

void MIDI_IO::sendRealtime(const midi::MidiType message) {
  serial.sendRealTime(message);
  usb.sendRealTime(message);
}

void MIDI_IO::sendControlChange(byte cc, byte value, byte channel) {
  serial.sendControlChange(cc, value, channel);
  usb.sendControlChange(cc, value, channel);
}

void MIDI_IO::sendNoteOn(byte inNoteNumber, byte inVelocity,
                              byte inChannel) {
  serial.sendNoteOn(inNoteNumber, inVelocity, inChannel);
  usb.sendNoteOn(inNoteNumber, inVelocity, inChannel);
}

void MIDI_IO::sendNoteOff(byte inNoteNumber, byte inVelocity,
                               byte inChannel) {
  serial.sendNoteOff(inNoteNumber, inVelocity, inChannel);
  usb.sendNoteOff(inNoteNumber, inVelocity, inChannel);
}

void MIDI_IO::sendSysEx(unsigned length, const byte *bytes) {
  serial.sendSysEx(length, bytes);
  usb.sendSysEx(length, bytes);
}
