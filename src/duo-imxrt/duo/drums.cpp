#include "drums.h"
#include "duo-firmware/src/DrumSynth.h"


void Drums::init(){
    drum_init();
}

void Drums::update(){
    drum_init();
}

static TouchState touches;
static TouchState previousTouches;

void Drums::update(){
  if(!BS814A_readRaw(touches)){
    return;
  }

#define KEY_PRESSED(key) (touches.key && !previousTouches.key)

  if (KEY_PRESSED(key3)) {
    kick_noteon(50);
  }
  if (KEY_PRESSED(key4)) {
    kick_noteon(127);
  }
  if (KEY_PRESSED(key2)) {
    hat_noteon(30);
  }
  if (KEY_PRESSED(key1)) {
    hat_noteon(127);
  }
#undef KEY_PRESSED

  previousTouches = touches;
  }
