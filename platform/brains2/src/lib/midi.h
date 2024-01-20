#ifndef MIDI_H_Z6SY8IRY
#define MIDI_H_Z6SY8IRY

#include "Arduino.h" // For HardwareSerial
#include <MIDI.h>
#include <USB-MIDI.h>

namespace MIDI_IO {
typedef void (*VoidCallback)();
typedef void (*SyxCallback)(byte *data, unsigned length);
typedef void (*NoteCallback)(uint8_t channel, uint8_t note, uint8_t velocity);

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

}; // namespace MIDI_IO

#endif /* end of include guard: MIDI_H_Z6SY8IRY */
