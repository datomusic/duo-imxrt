#ifndef DRUMS_H_D3BUGV50
#define DRUMS_H_D3BUGV50

#include "../../pins.h"

namespace Drums {

bool hat_l = 0;
bool hat_m = 0;
bool hat_r = 0;
bool kick_l = 0;
bool kick_m = 0;
bool kick_r = 0;
  
void init() {
  drum_init();
}

void update() {
  static unsigned long update_time = millis() + 10;

  if(millis() > update_time) {
    update_time = millis() + 10;
    hat_l = pinRead(HAT_PAD_L_PIN);
    hat_m = pinRead(HAT_PAD_M_PIN);
    hat_r = pinRead(HAT_PAD_R_PIN);
    kick_l = pinRead(KICK_PAD_L_PIN);
    kick_m = pinRead(KICK_PAD_M_PIN);
    kick_r = pinRead(KICK_PAD_R_PIN);

    if (hat_playing) {
      if (!hat_l && !hat_r && !hat_m) {
        hat_noteoff();
      }
    } else {
      if (hat_l && hat_m) {
        hat_noteon(31);
      } else if (hat_l) {
        hat_noteon(0);
      } else if (hat_r && hat_m) {
        hat_noteon(95);
      } else if (hat_r) {
        hat_noteon(127);
      } else if (hat_m) {
        hat_noteon(63);
      }
    } 

    if (kick_playing) {
      if (!kick_l && !kick_r && !kick_m) {
        kick_noteoff();
      }
    } else {
      if (kick_l && kick_m) {
        kick_noteon(31);
      } else if (kick_l) {
        kick_noteon(0);
      } else if (kick_r && kick_m) {
        kick_noteon(95);
      } else if (kick_r) {
        kick_noteon(127);
      } else if (kick_m) {
        kick_noteon(63);
      }
    }
  }
 }
} // namespace Drums

#endif /* end of include guard: DRUMS_H_D3BUGV50 */
