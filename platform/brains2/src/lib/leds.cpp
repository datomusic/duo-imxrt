#include "leds.h"
#include "fsl_common_arm.h"
#include "fsl_edma.h"
#include "fsl_flexio.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "teensy_audio_config.h"
#include <Arduino.h>
#include <Audio.h>

static uint8_t _brightness = 255;
static CRGB correction = 0xFFD0D0;

extern "C" void no_interrupts() {
  DisableIRQ(DMA0_IRQn);
  __disable_irq();
}

extern "C" void yes_interrupts() {
  EnableIRQ(DMA0_IRQn);
  __enable_irq();
}

extern "C" void show_pixels_bytes(uint32_t size, const uint8_t *bytes);

static edma_config_t dma_config = {0};

void init_dma() {
  EDMA_GetDefaultConfig(&dma_config);
  EDMA_Init(DMA0, &dma_config);
}

static void init_flexio() {
  flexio_config_t fxioUserConfig;
  FLEXIO_GetDefaultConfig(&fxioUserConfig);
  FLEXIO_Init(FLEXIO1, &fxioUserConfig);
}

namespace LEDs {

void setBrightness(int brightness) { _brightness = brightness; }
void init(void) {
  init_flexio();
  init_dma();
}


void show(const Pixel *const pixels, uint8_t pixel_count) {
  // no_interrupts();
  show_pixels_bytes(pixel_count * 3, (uint8_t *)pixels);
  // yes_interrupts();
}

} // namespace LEDs
