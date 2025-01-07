#ifndef AUDIO_H_OVLTWHKY
#define AUDIO_H_OVLTWHKY

namespace Audio {
  void headphone_enable(void);
  void headphone_disable(void);
  void amp_init(void);
  void amp_enable(void);
  void amp_disable(void);
  extern bool amp_enable_polarity;
} // namespace Audio

#endif /* end of include guard: AUDIO_H_OVLTWHKY */
