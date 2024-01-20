#ifndef MIDI_H_Z6SY8IRY
#define MIDI_H_Z6SY8IRY

#include <MIDI.h>

namespace MIDI_IO {
typedef void(VoidCallback)();
typedef void(SyxCallback)(byte *data, unsigned length);
typedef void(NoteCallback)(byte channel, byte note, byte velocity);

struct Callbacks {
  NoteCallback &note_on;
  NoteCallback &note_off;
  VoidCallback &clock;
  VoidCallback &start;
  VoidCallback &stop;
  VoidCallback &cont;
  NoteCallback &cc;
  SyxCallback &sysex;
};

void init(const byte channel, const Callbacks &callbacks);
void read(const byte channel);
void sendRealtime(const midi::MidiType message);
void sendControlChange(byte cc, byte value, byte channel);
void sendNoteOn(byte inNoteNumber, byte inVelocity, byte inChannel);
void sendNoteOff(byte inNoteNumber, byte inVelocity, byte inChannel);
void sendSysEx(unsigned length, const byte *bytes);

}; // namespace MIDI_IO

#endif /* end of include guard: MIDI_H_Z6SY8IRY */