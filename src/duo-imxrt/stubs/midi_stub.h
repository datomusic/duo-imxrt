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
  static void setHandleControlChange(void (*)(uint8_t, uint8_t, uint8_t)) {}
  static void setHandleNoteOn(void (*)(uint8_t, uint8_t, uint8_t)) {}
  static void setHandleNoteOff(void (*)(uint8_t, uint8_t, uint8_t)) {}
  static void setHandleClock(void (*)()) {}
  static void begin(int){}
  static void read(int){}
};

#endif /* end of include guard: MIDI_STUB_H_5CQHLM80 */
