#include "app.h"

int main(void) {
  App::init(MIDI::Callbacks{});

  const byte midi_channel = 1;
  int counter = 0;
  int note = 0;
  while (true) {
    if (counter++ > 100000) {
      counter = 0;
      note++;
      MIDI::sendNoteOn(note % 100, 100, 1);
    }

    App::update(midi_channel);
  }

  return 0;
}
