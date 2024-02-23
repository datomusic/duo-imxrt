#include "lib/midi_wrapper.h"

namespace App {
void init(MIDI::Callbacks midi_callbacks);
void update(byte midi_channel);
} // namespace App
