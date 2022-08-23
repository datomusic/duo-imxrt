#include "neopixel_imxrt_1010.h"

#define NEOPIXEL_PORT GPIO2
#define NEOPIXEL_PIN 5

void neopixel_imxrt1010_show(uint8_t _pin, uint8_t *pixels, uint32_t numBytes) {
#define CYCLES_800_T0H (F_CPU_ACTUAL / 4000000)
#define CYCLES_800_T1H (F_CPU_ACTUAL / 1250000)
#define CYCLES_800 (F_CPU_ACTUAL / 800000)

  uint8_t *p = pixels, *end = p + numBytes, pix, mask;
  uint32_t cyc;

  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
  cyc = ARM_DWT_CYCCNT;
  while (p < end) {
    pix = *p++;
    for (mask = 0x80; mask; mask >>= 1) {
      while (ARM_DWT_CYCCNT - cyc < CYCLES_800)
        ;
      cyc = ARM_DWT_CYCCNT;
      GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 1);
      if (pix & mask) {
        while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T1H)
          ;
      } else {
        while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T0H)
          ;
      }
      GPIO_PinWrite(NEOPIXEL_PORT, NEOPIXEL_PIN, 0);
    }
  }
  while (ARM_DWT_CYCCNT - cyc < CYCLES_800)
    ;
}
