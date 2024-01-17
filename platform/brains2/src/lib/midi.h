#ifndef MIDI_H_Z6SY8IRY
#define MIDI_H_Z6SY8IRY

#include "Arduino.h" // For HardwareSerial
#include <MIDI.h>
#include <USB-MIDI.h>

struct MIDIActuator {
  MIDIActuator();

  void init(const byte channel) {
    serial.begin(channel);
    usb.begin(channel);
  }

  void handle(const byte channel) {
    serial.read(channel);
    usb.read(channel);
  }

  typedef void (*VoidCallback)();
  typedef void (*SyxCallback)(byte *data, unsigned length);
  typedef void (*NoteCallback)(uint8_t channel, uint8_t note, uint8_t velocity);

  void setHandleNoteOn(NoteCallback callback) {
    serial.setHandleNoteOn(callback);
    usb.setHandleNoteOn(callback);
  }
  void setHandleNoteOff(NoteCallback callback) {
    serial.setHandleNoteOff(callback);
    usb.setHandleNoteOff(callback);
  }

  void setHandleControlChange(NoteCallback callback) {
    serial.setHandleControlChange(callback);
    usb.setHandleControlChange(callback);
  }

  void setHandleStart(VoidCallback callback) {
    serial.setHandleStart(callback);
    usb.setHandleStart(callback);
  }

  void setHandleStop(VoidCallback callback) {
    serial.setHandleStop(callback);
    usb.setHandleStop(callback);
  }

  void setHandleContinue(VoidCallback callback) {
    serial.setHandleContinue(callback);
    usb.setHandleContinue(callback);
  }

  void setHandleClock(VoidCallback callback) {
    usb.setHandleClock(callback);
  }

  void setHandleSystemExclusive(SyxCallback callback) {
    usb.setHandleSystemExclusive(callback);
  }

  void sendRealtime(const midi::MidiType message) {
    serial.sendRealTime(message);
    usb.sendRealTime(message);
  }

  void sendControlChange(byte cc, byte value, byte channel) {
    serial.sendControlChange(cc, value, channel);
    usb.sendControlChange(cc, value, channel);
  }

  void sendNoteOn(byte inNoteNumber, byte inVelocity, byte inChannel) {
    serial.sendNoteOn(inNoteNumber, inVelocity, inChannel);
    usb.sendNoteOn(inNoteNumber, inVelocity, inChannel);
  }

  void sendNoteOff(byte inNoteNumber, byte inVelocity, byte inChannel) {
    serial.sendNoteOff(inNoteNumber, inVelocity, inChannel);
    usb.sendNoteOff(inNoteNumber, inVelocity, inChannel);
  }

private:
  USBMIDI_NAMESPACE::usbMidiTransport usbTransport;
  MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> usb;

  MIDI_NAMESPACE::SerialMIDI<HardwareSerial> serialTransport;
  MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>>
      serial;
};

#endif /* end of include guard: MIDI_H_Z6SY8IRY */
