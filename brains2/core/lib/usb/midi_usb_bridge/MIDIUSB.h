#ifndef MIDIUSB_H_MY9N8UHO
#define MIDIUSB_H_MY9N8UHO

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t header;
  uint8_t byte1;
  uint8_t byte2;
  uint8_t byte3;
} midiEventPacket_t;

struct MidiUSB_ {
  midiEventPacket_t read(void);
  void flush(void);
  void sendMIDI(midiEventPacket_t event);
};

extern MidiUSB_ MidiUSB;

#endif /* end of include guard: MIDIUSB_H_MY9N8UHO */
