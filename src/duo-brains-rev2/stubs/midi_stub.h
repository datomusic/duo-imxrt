#ifndef MIDI_STUB_H_5CQHLM80
#define MIDI_STUB_H_5CQHLM80

struct MidiStub {
  enum {
    Clock,
    Start,
    Continue,
    Stop,
  };

  static void sendNoteOn(int, int, int) {}
  static void sendNoteOff(int, int, int) {}
  static void sendRealTime(int x) {}
  static void sendControlChange(int, int, int) {}
  static void setHandleStart(void (*)()) {}
  static void setHandleStop(void (*)()) {}
  static void setHandleContinue(void (*)()) {}
};

#endif /* end of include guard: MIDI_STUB_H_5CQHLM80 */
