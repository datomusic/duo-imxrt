#ifndef MIDI_STUB_H_5CQHLM80
#define MIDI_STUB_H_5CQHLM80

struct midi {
  enum {
    Clock,
    Start,
    Continue,
    Stop,
  };

  static void sendRealTime(int x) {}
  static void sendControlChange(int, int, int) {}
};

midi MIDI;
midi usbMIDI;

#endif /* end of include guard: MIDI_STUB_H_5CQHLM80 */
