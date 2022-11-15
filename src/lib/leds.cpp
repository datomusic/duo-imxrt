#include "leds.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include <Arduino.h>

#define NEOPIXEL_PINMUX IOMUXC_GPIO_SD_05_GPIO2_IO05
#define NEOPIXEL_PORT GPIO2
#define NEOPIXEL_PIN 5

#define DELTA_NS 70

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

#define _FASTLED_NS_TO_DWT(_NS)                                                \
  (((SystemCoreClock >> 16) * (_NS)) / (1000000000UL >> 16))

static inline uint32_t send_bit(uint8_t bit) {
#define MAGIC_800_INT 900'000   // ~1.11 us -> 1.2  field
#define MAGIC_800_T0H 2'800'000 // ~0.36 us -> 0.44 field
#define MAGIC_800_T1H 1'350'000 // ~0.74 us -> 0.84 field
#define MAGIC_800_RST 4000      // 80 us reset time
  /*   const uint32_t interval = SystemCoreClock / MAGIC_800_INT; */
  /*   const uint32_t t0 = SystemCoreClock / MAGIC_800_T0H; */
  /*   const uint32_t t1 = SystemCoreClock / MAGIC_800_T1H; */

  /* const uint32_t cycle = NS_TO_CYCLES(160); */
  /* const uint32_t delta = NS_TO_CYCLES(70); */

  const uint32_t t0 = NS_TO_CYCLES(300);
  const uint32_t t1 = NS_TO_CYCLES(600);
  const uint32_t interval = NS_TO_CYCLES(1000);

  const uint32_t cyc = bit ? t1 : t0;
  const auto start = DWT->CYCCNT;

  const uint32_t T1 = 250;
  const uint32_t T2 = 625;
  const uint32_t T3 = 375;

  const uint32_t off0 = _FASTLED_NS_TO_DWT(T1 + T2 + T3);
  const uint32_t off1 = _FASTLED_NS_TO_DWT(T2 + T3);
  const uint32_t off2 = _FASTLED_NS_TO_DWT(T3);

  /* const uint32_t off0 = SystemCoreClock / MAGIC_800_INT; */
  /* const uint32_t off1 = SystemCoreClock / MAGIC_800_T0H; */
  /* const uint32_t off2 = SystemCoreClock / MAGIC_800_T1H; */

  const uint32_t next_mark = DWT->CYCCNT + off0;
  pin_hi();

  if (bit) {
    while ((next_mark - DWT->CYCCNT) > off2)
      ;
  } else {
    while ((next_mark - DWT->CYCCNT) > off1)
      ;
  }

  pin_lo();

  while (DWT->CYCCNT < next_mark)
    ;

  const uint32_t next_cyc = bit ? t1 : t0;
  return next_cyc;
}

static inline uint32_t send_byte(uint8_t byte, uint32_t) {
  uint32_t mask = 0x80;

  uint32_t next_cyc;

  while (mask) {
    const uint8_t bit = byte & mask;
    next_cyc = send_bit(bit);
    mask >>= 1;
  }

  return next_cyc;
}

static inline uint32_t _send_byte(uint8_t byte, uint32_t next_mark) {
  const uint32_t delta = NS_TO_CYCLES(DELTA_NS);
  const uint32_t t0 = delta + NS_TO_CYCLES(160);
  const uint32_t t1 = delta + t0 + NS_TO_CYCLES(160);

#define BITS 8
  for (register uint32_t i = BITS - 1; i > 0; --i) {
    while (DWT->CYCCNT < next_mark)
      ;

    next_mark = DWT->CYCCNT + delta;
    pin_hi();

    if (byte & 0x80) {
      while ((next_mark - DWT->CYCCNT) > t1)
        ;
      pin_lo();
    } else {
      while ((next_mark - DWT->CYCCNT) > t0)
        ;
      pin_lo();
    }

    byte <<= 1;
  }

  while (DWT->CYCCNT < next_mark)
    ;
  next_mark = DWT->CYCCNT + delta;
  pin_hi();

  if (byte & 0x80) {
    while ((next_mark - DWT->CYCCNT) > t1)
      ;
    pin_lo();
  } else {
    while ((next_mark - DWT->CYCCNT) > t0)
      ;
    pin_lo();
  }

  return next_mark;
}

void show(const Pixel *const pixels, const int pixel_count) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  // assumes 800_000Hz frequency
  // Theoretical values here are 800_000 -> 1.25us, 2500000->0.4us,
  // 1250000->0.8us
  // TODO: try to get dynamic weighting working again

  const uint8_t *const pixel_bytes = (uint8_t *)pixels;
  const uint8_t *byte_ptr = pixel_bytes;
  const uint32_t byte_count = pixel_count * 3;
  const uint8_t *const end = pixel_bytes + byte_count;

  /* const uint32_t last = DWT->CYCCNT; */

  __disable_irq();
  uint32_t next_mark = DWT->CYCCNT + NS_TO_CYCLES(DELTA_NS);
  while (byte_ptr != end) {
    next_mark = send_byte(*byte_ptr, next_mark);
    byte_ptr++;

    next_mark = send_byte(*byte_ptr, next_mark);
    byte_ptr++;

    next_mark = send_byte(*byte_ptr, next_mark);
    byte_ptr++;

    /* if (DWT->CYCCNT - last > next_cyc) { */
    /*   break; */
    /* } */
    /* __disable_irq(); */
  }

  const uint32_t start = DWT->CYCCNT;
  __enable_irq();
  const uint32_t rst = NS_TO_CYCLES(100000);

  while ((DWT->CYCCNT - start) < rst)
    ;
}
} // namespace LEDs
