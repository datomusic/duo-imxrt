#include "leds.h"
#include "fsl_common_arm.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "teensy_audio_config.h"
#include <Arduino.h>
#include <Audio.h>

#define NEOPIXEL_PINMUX IOMUXC_GPIO_SD_05_GPIO2_IO05
#define NEOPIXEL_PORT GPIO2
#define NEOPIXEL_PIN 5

static uint32_t no_interrupts() {
  DisableIRQ(DMA0_IRQn);
  __disable_irq();
  return 0;
  /* return DisableGlobalIRQ(); */
}

/* static void yes_interrupts(const uint32_t primask) { */
static void yes_interrupts() {
  /* EnableGlobalIRQ(primask); */
  __enable_irq();
  EnableIRQ(DMA0_IRQn);
}

inline static void pin_hi() { NEOPIXEL_PORT->DR |= (1UL << NEOPIXEL_PIN); }
inline static void pin_lo() { NEOPIXEL_PORT->DR &= ~(1UL << NEOPIXEL_PIN); }

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

static inline void send_bit(uint8_t bit, uint32_t &next_mark, uint32_t *off) {}

static inline uint32_t send_byte(uint8_t byte, uint32_t &next_mark,
                                 uint32_t *off) {
  uint32_t mask = 0x80;

  while (mask) {
    const uint8_t bit = byte & mask;

    while (DWT->CYCCNT < next_mark)
      ;

    const uint32_t on_cycles = bit ? off[1] : off[2];

    next_mark = DWT->CYCCNT + off[0];
    const uint32_t on_cutoff = next_mark - (off[0] - on_cycles);

    pin_hi();

    while (DWT->CYCCNT < on_cutoff)
      ;

    pin_lo();

    mask >>= 1;
  }
}

template <int WAIT> class CMinWait {
  uint16_t mLastMicros;

public:
  CMinWait() { mLastMicros = 0; }

  void wait() {
    uint16_t diff;
    do {
      diff = (micros() & 0xFFFF) - mLastMicros;
    } while (diff < WAIT);
  }

  void mark() { mLastMicros = micros() & 0xFFFF; }
};

#define WAIT_TIME 50

CMinWait<WAIT_TIME> mWait;

static uint32_t show_pixels(const Pixel *const pixels, const int pixel_count) {
  const uint32_t start = DWT->CYCCNT;
  // assumes 800_000Hz frequency
  // Theoretical values here are 800_000 -> 1.25us, 2500000->0.4us,
  // 1250000->0.8us
  // TODO: try to get dynamic weighting working again

  const Pixel *const end = pixels + pixel_count;
  const Pixel *pixel_ptr = pixels;

  /* const uint32_t last = DWT->CYCCNT; */

#define NS_PER_SEC                                                             \
  (1000000000L) // Note that this has to be SIGNED since we want to be able to
                // check for negative values of derivatives
#define CYCLES_PER_SEC (SystemCoreClock)
#define NS_PER_CYCLE (NS_PER_SEC / CYCLES_PER_SEC)
#define NS_TO_CYCLES(n) ((n) / NS_PER_CYCLE)

#define T1 250
#define T2 625
#define T3 375
  uint32_t off[3];
  off[0] = NS_TO_CYCLES(T1 + T2 + T3);
  off[1] = NS_TO_CYCLES(T2 + T3);
  off[2] = NS_TO_CYCLES(T3);

#define INTERRUPT_THRESHOLD 1

#define ALLOW_INTERRUPTS

  const uint32_t wait_off =
      NS_TO_CYCLES((WAIT_TIME - INTERRUPT_THRESHOLD) * 1000);

  pin_lo();

  no_interrupts();

  uint32_t next_mark = DWT->CYCCNT + off[0];

  while (pixel_ptr != end) {
#ifdef ALLOW_INTERRUPTS
    no_interrupts();

    if (DWT->CYCCNT > next_mark) {
      if ((DWT->CYCCNT - next_mark) > wait_off) {
        yes_interrupts();
        return false;
      }
    }
#endif
      
    const Pixel pix = *(pixel_ptr++);

    send_byte(pix.g, next_mark, off);
    send_byte(pix.r, next_mark, off);
    send_byte(pix.b, next_mark, off);

#ifdef ALLOW_INTERRUPTS
    yes_interrupts();
#endif
  }

  pin_lo();

  yes_interrupts();
  return true;
}

void show(const Pixel *const pixels, const int pixel_count) {
  /* mWait.wait(); */
  if (!show_pixels(pixels, pixel_count)) {
    yes_interrupts();
    no_interrupts();
    show_pixels(pixels, pixel_count);
  }

  /* mWait.mark(); */
}

} // namespace LEDs
