#ifndef ROMPLER_H_A3V9GTDX
#define ROMPLER_H_A3V9GTDX

#include "AudioSampleGong.h"
#include "AudioSampleHihat.h"
#include "AudioSampleKick.h"
#include "AudioSampleSnare.h"
#include "AudioSampleTomtom.h"
#include <mixer.h>
#include <play_memory.h>

namespace Rompler {
struct Sound {
  Sound(const unsigned int *sample_data) : sample_data(sample_data) {
  }

  void play() {
    player.play(sample_data);
  }

  AudioPlayMemory player;

private:
  const unsigned int *const sample_data;
};

Sound kick(AudioSampleKick);
Sound snare(AudioSampleSnare);
Sound hihat(AudioSampleHihat);
Sound tom(AudioSampleTomtom);
AudioMixer4 mixer;

static AudioConnection connections[] = {
    AudioConnection(snare.player, 0, mixer, 0),
    AudioConnection(kick.player, 0, mixer, 1),
    AudioConnection(hihat.player, 0, mixer, 2),
    AudioConnection(tom.player, 0, mixer, 3),
};

AudioStream &get_output() {
  return mixer;
};

}; // namespace Rompler

#endif /* end of include guard: ROMPLER_H_A3V9GTDX */
