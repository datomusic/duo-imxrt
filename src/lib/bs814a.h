#ifndef BS814A_H_93NMBE2I
#define BS814A_H_93NMBE2I

#include <stdint.h>

namespace BS814A {
const auto KEY_COUNT = 4;

typedef uint8_t TouchState;

inline bool key_down(TouchState s, uint8_t index){
  return (s & (1 << index));
};

void init();
bool readRaw(TouchState *s);
} // namespace BS814A

#endif /* end of include guard: BS814A_H_93NMBE2I */
