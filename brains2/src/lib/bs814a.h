#ifndef BS814A_H_93NMBE2I
#define BS814A_H_93NMBE2I

#include <stdint.h>

namespace BS814A {
const auto KEY_COUNT = 4;

typedef uint8_t TouchState;

enum ButtonMask{
  Button1 = 1,
  Button2 = 2,
  Button3 = 4,
  Button4 = 8
};

void init();
bool readRaw(TouchState *s);
} // namespace BS814A

#endif /* end of include guard: BS814A_H_93NMBE2I */
