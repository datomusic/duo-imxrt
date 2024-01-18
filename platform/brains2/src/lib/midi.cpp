#include "midi.h"

MIDIActuator::MIDIActuator()
    : usbTransport(0), usb(usbTransport), serialTransport(Serial),
      serial(serialTransport) {
}

void MIDIActuator::init(const byte channel) {
  serial.begin(channel);
  usb.begin(channel);
}

void MIDIActuator::handle(const byte channel) {
  serial.read(channel);
  usb.read(channel);
}

void MIDIActuator::setHandleNoteOn(NoteCallback callback) {
  serial.setHandleNoteOn(callback);
  usb.setHandleNoteOn(callback);
}
void MIDIActuator::setHandleNoteOff(NoteCallback callback) {
  serial.setHandleNoteOff(callback);
  usb.setHandleNoteOff(callback);
}

void MIDIActuator::setHandleControlChange(NoteCallback callback) {
  serial.setHandleControlChange(callback);
  usb.setHandleControlChange(callback);
}

void MIDIActuator::setHandleStart(VoidCallback callback) {
  serial.setHandleStart(callback);
  usb.setHandleStart(callback);
}

void MIDIActuator::setHandleStop(VoidCallback callback) {
  serial.setHandleStop(callback);
  usb.setHandleStop(callback);
}

void MIDIActuator::setHandleContinue(VoidCallback callback) {
  serial.setHandleContinue(callback);
  usb.setHandleContinue(callback);
}

void MIDIActuator::setHandleClock(VoidCallback callback) {
  usb.setHandleClock(callback);
}

void MIDIActuator::setHandleSystemExclusive(SyxCallback callback) {
  usb.setHandleSystemExclusive(callback);
}

void MIDIActuator::sendRealtime(const midi::MidiType message) {
  serial.sendRealTime(message);
  usb.sendRealTime(message);
}

void MIDIActuator::sendControlChange(byte cc, byte value, byte channel) {
  serial.sendControlChange(cc, value, channel);
  usb.sendControlChange(cc, value, channel);
}

void MIDIActuator::sendNoteOn(byte inNoteNumber, byte inVelocity,
                              byte inChannel) {
  serial.sendNoteOn(inNoteNumber, inVelocity, inChannel);
  usb.sendNoteOn(inNoteNumber, inVelocity, inChannel);
}

void MIDIActuator::sendNoteOff(byte inNoteNumber, byte inVelocity,
                               byte inChannel) {
  serial.sendNoteOff(inNoteNumber, inVelocity, inChannel);
  usb.sendNoteOff(inNoteNumber, inVelocity, inChannel);
}

void MIDIActuator::sendSysEx(unsigned length, const byte *bytes) {
  serial.sendSysEx(length, bytes);
  usb.sendSysEx(length, bytes);
}
