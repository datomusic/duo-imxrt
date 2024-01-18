#ifndef MIDI_H_Z6SY8IRY
#define MIDI_H_Z6SY8IRY

#include "Arduino.h" // For HardwareSerial
#include <MIDI.h>
#include <USB-MIDI.h>

struct MIDIActuator {
  typedef void (*VoidCallback)();
  typedef void (*SyxCallback)(byte *data, unsigned length);
  typedef void (*NoteCallback)(uint8_t channel, uint8_t note, uint8_t velocity);

  MIDIActuator();
  void init(const byte channel);
  void handle(const byte channel);
  void setHandleNoteOn(NoteCallback callback);
  void setHandleNoteOff(NoteCallback callback);
  void setHandleControlChange(NoteCallback callback);
  void setHandleStart(VoidCallback callback);
  void setHandleStop(VoidCallback callback);
  void setHandleContinue(VoidCallback callback);
  void setHandleClock(VoidCallback callback);
  void setHandleSystemExclusive(SyxCallback callback);
  void sendRealtime(const midi::MidiType message);
  void sendControlChange(byte cc, byte value, byte channel);
  void sendNoteOn(byte inNoteNumber, byte inVelocity, byte inChannel);
  void sendNoteOff(byte inNoteNumber, byte inVelocity, byte inChannel);
  void sendSysEx(unsigned length, const byte *bytes);

  byte channel = 1;

private:
  USBMIDI_NAMESPACE::usbMidiTransport usbTransport;
  MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> usb;

  MIDI_NAMESPACE::SerialMIDI<HardwareSerial> serialTransport;
  MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>>
      serial;
};

#endif /* end of include guard: MIDI_H_Z6SY8IRY */
