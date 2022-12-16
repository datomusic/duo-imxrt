#ifndef __BS814A__
#define __BS814A__

namespace BS814A{
  const auto KEY_COUNT = 4;

  void init();
  void readRaw(const bool[KEY_COUNT]);
}

#endif
