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
  static BS814A::TouchState touches;

  if (!BS814A::readRaw(&touches)) {
    return;
  }
#define m0 1
#define m1 2
#define m2 4
#define m3 8

  if (hat_playing) {
    if (!((touches & m0) || (touches & m1))) {
      hat_noteoff();
    }
  } else {
    if ((touches & m0) && (touches & m1)) {
      hat_noteon(64);
    } else if ((touches & m0)) {
      hat_noteon(127);
    } else if ((touches & m1)) {
      hat_noteon(0);
    }
  }

  if (kick_playing) {
    if (!((touches & m2) || (touches & m3))) {
      kick_noteoff();
    }
  } else {
    if ((touches & m2) && (touches & m3)) {
      kick_noteon(64);
    } else if ((touches & m2)) {
      kick_noteon(0);
    } else if ((touches & m3)) {
      kick_noteon(127);
    }
  }
}
} // namespace Drums

#endif /* end of include guard: DRUMS_H_D3BUGV50 */
