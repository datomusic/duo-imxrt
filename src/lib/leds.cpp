#include "leds.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define NEOPIXEL_PINMUX IOMUXC_GPIO_SD_05_GPIO2_IO05
#define NEOPIXEL_PORT GPIO2
#define NEOPIXEL_PIN 5
#define NEOPIXEL_BRIGHTNESS 0xFF

#define MAGIC_800_INT 900000  // ~1.11 us -> 1.2  field
#define MAGIC_800_T0H 2800000 // ~0.36 us -> 0.44 field
#define MAGIC_800_T1H 1350000 // ~0.74 us -> 0.84 field

static inline uint8_t apply_percentage(const uint8_t brightness) {
  return (uint8_t)((brightness * NEOPIXEL_BRIGHTNESS) >> 8);
}

namespace LEDs {

void show(const Pixel *const pixels, const int pixel_count) {
  const uint8_t *const pixel_bytes = (uint8_t *)pixels;
  const uint32_t byte_count = pixel_count * 3;

  // assumes 800_000Hz frequency
  // Theoretical values here are 800_000 -> 1.25us, 2500000->0.4us,
  // 1250000->0.8us
  // TODO: try to get dynamic weighting working again
  const uint32_t sys_freq = SystemCoreClock;
  const uint32_t interval = sys_freq / MAGIC_800_INT;
  const uint32_t t0 = sys_freq / MAGIC_800_T0H;
  const uint32_t t1 = sys_freq / MAGIC_800_T1H;

  const uint8_t *p = pixel_bytes;
  const uint8_t *const end = p + byte_count;
  uint8_t pix = *p++;
  uint8_t mask = 0x80;
  uint32_t start = 0;
  uint32_t cyc = 0;

  __disable_irq();

  // Enable DWT in debug core. Useable when interrupts disabled, as opposed to
  // Systick->VAL
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  DWT->CYCCNT = 0;

  for (;;) {
    cyc = (pix & mask) ? t1 : t0;
    start = DWT->CYCCNT;

    GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 1);
    while ((DWT->CYCCNT - start) < cyc)
      ;

    GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 0);
    while ((DWT->CYCCNT - start) < interval)
      ;

    if (!(mask >>= 1)) {
      if (p >= end) {
        break;
      }

      pix = *p++;
      mask = 0x80;
    }
  }
  __enable_irq();
}

void init(void) {
  IOMUXC_SetPinMux(NEOPIXEL_PINMUX, 0U);
  IOMUXC_SetPinConfig(NEOPIXEL_PINMUX, 0x10B0U);

  gpio_pin_config_t neopixel_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
  GPIO_PinInit(NEOPIXEL_PORT, NEOPIXEL_PIN, &neopixel_config);
}
} // namespace LEDs
