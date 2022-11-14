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

#define ARM_DWT_CYCCNT DWT->CYCCNT

void pin_hi() { GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 1); }

void pin_lo() { GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 0); }

template <int BITS>
__attribute__((always_inline)) inline void
writeBits(const uint32_t *off, uint32_t &next_mark, uint8_t &b) {
  for (uint32_t i = BITS - 1; i > 0; --i) {
    while (ARM_DWT_CYCCNT < next_mark)
      ;
    next_mark = ARM_DWT_CYCCNT + off[0];
    pin_hi();
    if (b & 0x80) {
      while ((next_mark - ARM_DWT_CYCCNT) > off[2])
        ;
      pin_lo();
    } else {
      while ((next_mark - ARM_DWT_CYCCNT) > off[1])
        ;
      pin_lo();
    }
    b <<= 1;
  }

  while (ARM_DWT_CYCCNT < next_mark)
    ;
  next_mark = ARM_DWT_CYCCNT + off[0];
  pin_hi();

  if (b & 0x80) {
    while ((next_mark - ARM_DWT_CYCCNT) > off[2])
      ;
    pin_lo();
  } else {
    while ((next_mark - ARM_DWT_CYCCNT) > off[1])
      ;
    pin_lo();
  }
}

#define _FASTLED_NS_TO_DWT(_NS)                                                \
  (((SystemCoreClock >> 16) * (_NS)) / (1000000000UL >> 16))

static uint32_t show_pixels(const LEDs::Pixel *const pixels,
                            const int pixel_count) {
  const uint8_t *const pixel_bytes = (uint8_t *)pixels;
  uint32_t start = DWT->CYCCNT;

/* #define FMUL (SystemCoreClock / 8000000) */
#define C_NS(_NS) (((_NS * ((SystemCoreClock / 1000000L)) + 999)) / 1000)

  const int T1 = C_NS(300);
  const int T2 = C_NS(300);
  const int T3 = C_NS(600);

  uint32_t off[3];
  off[0] = _FASTLED_NS_TO_DWT(T1 + T2 + T3);
  off[1] = _FASTLED_NS_TO_DWT(T2 + T3);
  off[2] = _FASTLED_NS_TO_DWT(T3);

  __disable_irq();

  uint32_t next_mark = ARM_DWT_CYCCNT + off[0];

  for (int i = 0; i < pixel_count; ++i) {
    uint8_t b = pixel_bytes[i * 3];
    writeBits<8>(off, next_mark, b);
    b = pixel_bytes[i * 3 + 1];
    writeBits<8>(off, next_mark, b);
    b = pixel_bytes[i * 3 + 2];
    writeBits<8>(off, next_mark, b);
  }

  __enable_irq();
  return ARM_DWT_CYCCNT - start;
}

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

#define WAIT_TIME 50
void _show(const Pixel *const pixels, const int pixel_count) {
  /* if (!show_pixels(pixels, pixel_count)) { */
  /*   __enable_irq(); */
  /*   delayMicroseconds(WAIT_TIME); */
  /*   __disable_irq(); */
  show_pixels(pixels, pixel_count);
  /* } */
}

uint32_t write_byte(uint8_t pix, uint32_t cyc) {
  const uint32_t sys_freq = SystemCoreClock;
  const uint32_t interval = sys_freq / MAGIC_800_INT;
  const uint32_t t0 = sys_freq / MAGIC_800_T0H;
  const uint32_t t1 = sys_freq / MAGIC_800_T1H;

  uint8_t mask = 0x80;
  uint32_t start = 0;

  while (true) {
    cyc = (pix & mask) ? t1 : t0;
    start = DWT->CYCCNT;

    pin_hi();
    while ((DWT->CYCCNT - start) < cyc)
      ;

    pin_lo();
    while ((DWT->CYCCNT - start) < interval)
      ;

    if (!(mask >>= 1)) {
      return cyc;
    }
  }
}

void show(const Pixel *const pixels, const int pixel_count) {
  const uint8_t *const pixel_bytes = (uint8_t *)pixels;
  const uint32_t byte_count = pixel_count * 3;

  // assumes 800_000Hz frequency
  // Theoretical values here are 800_000 -> 1.25us, 2500000->0.4us,
  // 1250000->0.8us
  // TODO: try to get dynamic weighting working again

  const uint8_t *p = pixel_bytes;
  const uint8_t *const end = p + byte_count;
  const uint32_t sys_freq = SystemCoreClock;
  const uint32_t interval = sys_freq / MAGIC_800_INT;
  const uint32_t t0 = sys_freq / MAGIC_800_T0H;
  const uint32_t t1 = sys_freq / MAGIC_800_T1H;

  uint8_t mask = 0x80;
  uint32_t cyc = 0;
  uint32_t start = 0;
  uint8_t pix = *p++;
  const int INTERRUPT_THRESHOLD = 1;

  uint32_t wait_off =
      _FASTLED_NS_TO_DWT((WAIT_TIME - INTERRUPT_THRESHOLD) * 1000);

  __disable_irq();

  while (p != end) {
    __disable_irq();

    // if interrupts took longer than 45µs, punt on the current frame
    /* if (DWT->CYCCNT > interval) { */
    /*   if ((DWT->CYCCNT - interval) > wait_off) { */
    /*     __enable_irq(); */
    /*     /1* return DWT->CYCCNT - start; *1/ */
    /*     return; */
    /*   } */
    /* } */

    while (true) {
      cyc = (pix & mask) ? t1 : t0;
      start = DWT->CYCCNT;

      pin_hi();
      while ((DWT->CYCCNT - start) < cyc)
        ;

      pin_lo();
      while ((DWT->CYCCNT - start) < interval)
        ;

      if (!(mask >>= 1)) {
        mask = 0x80;
        break;
      }
    }

    __enable_irq();
    pix = *p++;
  }

  __disable_irq();

  start = DWT->CYCCNT;
  pin_lo();

  const uint32_t rst = sys_freq / MAGIC_800_RST;

  __enable_irq();
  while ((DWT->CYCCNT - start) < rst)
    ;
}
} // namespace LEDs
