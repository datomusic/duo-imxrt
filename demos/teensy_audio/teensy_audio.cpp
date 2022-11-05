#include "Arduino.h"
#include "Audio.h"
#include "lib/board_init.h"
#include "lib/clock_config.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "lib/pin_mux.h"

static void headphone_enable(void) {
#define HP_ENABLE_PINMUX IOMUXC_GPIO_AD_11_GPIOMUX_IO25
#define HP_ENABLE_PORT GPIO1
#define HP_ENABLE_PIN 25U

  IOMUXC_SetPinMux(HP_ENABLE_PINMUX, 0U);
  gpio_pin_config_t hp_enable_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(HP_ENABLE_PORT, HP_ENABLE_PIN, &hp_enable_config);
  GPIO_PinWrite(HP_ENABLE_PORT, HP_ENABLE_PIN, 1);
}

static void amp_enable(void) {
#define AMP_MUTE_PINMUX IOMUXC_GPIO_AD_10_GPIOMUX_IO24
#define AMP_MUTE_PORT GPIO1
#define AMP_MUTE_PIN 24U

  IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
  gpio_pin_config_t amp_mute_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
  GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, 0);
}

int main(void) {
  board_init();
  amp_enable();

  pinMode(PIN_SYNC_OUT, OUTPUT);

  AudioSynthWaveformSine sine1; // xy=174,384
  AudioOutputMQS mqs1;          // xy=356,386
  AudioConnection patchCord1(sine1, 0, mqs1, 0);
  AudioConnection patchCord2(sine1, 0, mqs1, 1);

  AudioNoInterrupts();
  AudioMemory(64);
  sine1.frequency(440);
  sine1.amplitude(0.1);
  AudioInterrupts();

  unsigned long sweep_update_time = millis() + 10;

  while (true) {

    if (millis() > sweep_update_time) {
      uint32_t speed_pot = analogRead(PIN_POT_2);
      uint32_t length_pot = analogRead(PIN_POT_1);
      AudioNoInterrupts();
      sine1.amplitude((4096 - length_pot) / 16384.0);
      sine1.frequency(4100 - speed_pot);
      AudioInterrupts();
    }
  }
}
