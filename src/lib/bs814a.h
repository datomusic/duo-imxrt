#ifndef BS814A_H_93NMBE2I
#define BS814A_H_93NMBE2I

#include <stdint.h>

namespace BS814A {
const auto KEY_COUNT = 4;

struct TouchState {
  uint8_t key1;
  uint8_t key2;
  uint8_t key3;
  uint8_t key4;
};

void init();
bool readRaw(TouchState &s);
} // namespace BS814A

#endif /* end of include guard: BS814A_H_93NMBE2I */
