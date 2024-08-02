#ifndef AUDIO_OUTPUT_H_VS0HQTKT
#define AUDIO_OUTPUT_H_VS0HQTKT

#include <output_pt8211.h>

typedef AudioOutputPT8211 BoardAudioOutput;

const float HEADPHONE_GAIN = 2.0f;
const float SPEAKER_GAIN   = 2.0f;

const float HEADPHONE_KICK_GAIN = 0.5f;
const float HEADPHONE_HAT_GAIN = 0.5f;
const float HEADPHONE_DELAY_GAIN = 1.0f;
const float HEADPHONE_MAIN_GAIN = 0.8f;

const float SPEAKER_MAIN_GAIN = 0.8f;
const float SPEAKER_DELAY_GAIN = 1.0f;
const float SPEAKER_KICK_GAIN = 0.8f;
const float SPEAKER_HAT_GAIN = 1.0f;

float MAIN_GAIN      = 0.8f;
float DELAY_GAIN     = 1.0f;
float KICK_GAIN      = 1.0f;
float HAT_GAIN       = 1.2f;

#endif /* end of include guard: AUDIO_OUTPUT_H_VS0HQTKT */
