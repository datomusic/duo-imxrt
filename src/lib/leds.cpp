#include "leds.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include <Arduino.h>

#define NEOPIXEL_PINMUX IOMUXC_GPIO_SD_05_GPIO2_IO05
#define NEOPIXEL_PORT GPIO2
#define NEOPIXEL_PIN 5

#define MAGIC_800_INT 900'000   // ~1.11 us -> 1.2  field
#define MAGIC_800_T0H 2'800'000 // ~0.36 us -> 0.44 field
#define MAGIC_800_T1H 1'350'000 // ~0.74 us -> 0.84 field
#define MAGIC_800_RST 4000      // 80 us reset time

static void pin_hi() { GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 1); }
static void pin_lo() { GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 0); }

namespace LEDs {

void init(void) {
  IOMUXC_SetPinMux(NEOPIXEL_PINMUX, 0U);
  IOMUXC_SetPinConfig(NEOPIXEL_PINMUX, 0x10B0U);

  gpio_pin_config_t neopixel_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
  GPIO_PinInit(NEOPIXEL_PORT, NEOPIXEL_PIN, &neopixel_config);

  // Enable DWT in debug core. Useable when interrupts disabled, as opposed to
  // Systick->VAL
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

#define NS_PER_SEC                                                             \
  (1000000000L) // Note that this has to be SIGNED since we want to be able to
                // check for negative values of derivatives
#define CYCLES_PER_SEC (SystemCoreClock)
#define NS_PER_CYCLE (NS_PER_SEC / CYCLES_PER_SEC)
#define NS_TO_CYCLES(n) ((n) / NS_PER_CYCLE)

static inline void send_bit(uint8_t bit) {
  const uint32_t cycle = NS_TO_CYCLES(160);
  const uint32_t delta = NS_TO_CYCLES(70);

  const uint32_t t0 = cycle + delta;
  const uint32_t t1 = t0 + 3 * cycle;
  const uint32_t interval = t1 + t0 + delta;

  const uint32_t cyc = bit ? t1 : t0;
  const auto start = DWT->CYCCNT;

  pin_hi();
  while ((DWT->CYCCNT - start) < cyc)
    ;

  pin_lo();
  while ((DWT->CYCCNT - start) < interval)
    ;
}

static inline void send_byte(uint8_t byte) {
  uint32_t mask = 0x80;

  while (mask) {
    const uint8_t bit = byte & mask;
    send_bit(bit);
    mask >>= 1;
  }
}

void show(const Pixel *const pixels, const int pixel_count) {

  // assumes 800_000Hz frequency
  // Theoretical values here are 800_000 -> 1.25us, 2500000->0.4us,
  // 1250000->0.8us
  // TODO: try to get dynamic weighting working again

  const uint8_t *const pixel_bytes = (uint8_t *)pixels;
  const uint8_t *byte_ptr = pixel_bytes;
  const uint32_t byte_count = pixel_count * 3;
  const uint8_t *const end = pixel_bytes + byte_count;

  const uint32_t last = DWT->CYCCNT;
  __disable_irq();
  while (byte_ptr != end) {

    send_byte(*byte_ptr);
    byte_ptr++;
    send_byte(*byte_ptr);
    byte_ptr++;
    send_byte(*byte_ptr);
    byte_ptr++;

    uint32_t last = DWT->CYCCNT;
    __enable_irq();

    const uint32_t cycle = NS_TO_CYCLES(160);
    const uint32_t delta = NS_TO_CYCLES(70);

    const uint32_t t0 = cycle + delta;
    const uint32_t t1 = t0 + 3 * cycle;
    const uint32_t interval = t1 + t0 + delta;

    __disable_irq();
    if (DWT->CYCCNT - last > interval) {
      break;
    }
  }

  __enable_irq();

  const uint32_t start = DWT->CYCCNT;
  const uint32_t rst = SystemCoreClock / MAGIC_800_RST;

  while ((DWT->CYCCNT - start) < rst)
    ;
}
} // namespace LEDs
