#ifndef BS814A_H_93NMBE2I
#define BS814A_H_93NMBE2I

#include <stdint.h>

namespace BS814A {
const auto KEY_COUNT = 4;

typedef uint8_t TouchState;

inline bool down(TouchState state, uint8_t index){
  return (state & (1 << index));
};

inline bool pressed(TouchState state, TouchState previous, uint8_t index){
  const int mask = (1 << index);
  return (state & mask) && !(previous & mask) ;
};

void init();
bool readRaw(TouchState *s);
} // namespace BS814A

#endif /* end of include guard: BS814A_H_93NMBE2I */
