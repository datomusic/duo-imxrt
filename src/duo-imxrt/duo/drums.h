#ifndef DRUMS_H_D3BUGV50
#define DRUMS_H_D3BUGV50

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

  if (hat_playing) {
    if (!((touches & BS814A::Button1) || (touches & BS814A::Button2))) {
      hat_noteoff();
    }
  } else {
    if ((touches & BS814A::Button1) && (touches & BS814A::Button2)) {
      hat_noteon(64);
    } else if ((touches & BS814A::Button1)) {
      hat_noteon(0);
    } else if ((touches & BS814A::Button2)) {
      hat_noteon(127);
    }
  }

  if (kick_playing) {
    if (!((touches & BS814A::Button3) || (touches & BS814A::Button4))) {
      kick_noteoff();
    }
  } else {
    if ((touches & BS814A::Button3) && (touches & BS814A::Button4)) {
      kick_noteon(64);
    } else if ((touches & BS814A::Button3)) {
      kick_noteon(0);
    } else if ((touches & BS814A::Button4)) {
      kick_noteon(127);
    }
  }
}
} // namespace Drums

#endif /* end of include guard: DRUMS_H_D3BUGV50 */
