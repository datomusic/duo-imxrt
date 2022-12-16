#ifndef DRUMS_H_D3BUGV50
#define DRUMS_H_D3BUGV50

#include "duo-firmware/src/DrumSynth.h"
#include "lib/bs814a.h"

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

  if (BS814A::pressed(touches, previousTouches, 0)) {
    hat_noteon(127);
  }

  if (BS814A::pressed(touches, previousTouches, 1)) {
    hat_noteon(30);
  }

  if (BS814A::pressed(touches, previousTouches, 2)) {
    kick_noteon(50);
  }

  if (BS814A::pressed(touches, previousTouches, 3)) {
    kick_noteon(127);
  }

  previousTouches = touches;
}
} // namespace Drums

#endif /* end of include guard: DRUMS_H_D3BUGV50 */
