#include "neopixel_imxrt_1010.h"

void neopixel_imxrt1010_show(uint8_t pin, uint8_t *pixels, uint32_t numBytes,
                             bool is800KHz) {
#define CYCLES_800_T0H (F_CPU_ACTUAL / 4000000)
#define CYCLES_800_T1H (F_CPU_ACTUAL / 1250000)
#define CYCLES_800 (F_CPU_ACTUAL / 800000)
#define CYCLES_400_T0H (F_CPU_ACTUAL / 2000000)
#define CYCLES_400_T1H (F_CPU_ACTUAL / 833333)
#define CYCLES_400 (F_CPU_ACTUAL / 400000)

  uint8_t *p = pixels, *end = p + numBytes, pix, mask;
  volatile uint32_t *set = portSetRegister(pin), *clr = portClearRegister(pin);
  uint32_t cyc, msk = digitalPinToBitMask(pin);

  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

#if defined(NEO_KHZ400) // 800 KHz check needed only if 400 KHz support enabled
  if (is800KHz) {
#endif
    cyc = ARM_DWT_CYCCNT + CYCLES_800;
    while (p < end) {
      pix = *p++;
      for (mask = 0x80; mask; mask >>= 1) {
        while (ARM_DWT_CYCCNT - cyc < CYCLES_800)
          ;
        cyc = ARM_DWT_CYCCNT;
        *set = msk;
        if (pix & mask) {
          while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T1H)
            ;
        } else {
          while (ARM_DWT_CYCCNT - cyc < CYCLES_800_T0H)
            ;
        }
        *clr = msk;
      }
    }
    while (ARM_DWT_CYCCNT - cyc < CYCLES_800)
      ;
#if defined(NEO_KHZ400)
  } else { // 400 kHz bitstream
    cyc = ARM_DWT_CYCCNT + CYCLES_400;
    while (p < end) {
      pix = *p++;
      for (mask = 0x80; mask; mask >>= 1) {
        while (ARM_DWT_CYCCNT - cyc < CYCLES_400)
          ;
        cyc = ARM_DWT_CYCCNT;
        *set = msk;
        if (pix & mask) {
          while (ARM_DWT_CYCCNT - cyc < CYCLES_400_T1H)
            ;
        } else {
          while (ARM_DWT_CYCCNT - cyc < CYCLES_400_T0H)
            ;
        }
        *clr = msk;
      }
    }
    while (ARM_DWT_CYCCNT - cyc < CYCLES_400)
      ;
  }
#endif // NEO_KHZ400
}
