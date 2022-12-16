#ifndef DRUMS_H_D3BUGV50
#define DRUMS_H_D3BUGV50

#include "lib/bs814a.h"
#include "duo-firmware/src/DrumSynth.h"

namespace Drums {
void init() {
  BS814A::init();
  drum_init();
}


void update() {
  static BS814A::TouchState previousTouches;
  static BS814A::TouchState touches;

  if (!BS814A::readRaw(&touches)) {
    return;
  }

#define KEY_PRESSED(index) (BS814A::key_down(touches, index) && !BS814A::key_down(previousTouches, index))

  if (KEY_PRESSED(2)) {
    kick_noteon(50);
  }
  if (KEY_PRESSED(3)) {
    kick_noteon(127);
  }
  if (KEY_PRESSED(1)) {
    hat_noteon(30);
  }
  if (KEY_PRESSED(0)) {
    hat_noteon(127);
  }
#undef KEY_PRESSED

  previousTouches = touches;
}
} // namespace Drums

#endif /* end of include guard: DRUMS_H_D3BUGV50 */
