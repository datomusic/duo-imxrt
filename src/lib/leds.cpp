#include "leds.h"
#include "fsl_common_arm.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "teensy_audio_config.h"
#include <Arduino.h>
#include <Audio.h>

#define ALLOW_INTERRUPTS

#define INTERRUPT_THRESHOLD 1
#define WAIT_MICROSECONDS 80

#define NEOPIXEL_PINMUX IOMUXC_GPIO_SD_05_GPIO2_IO05
#define NEOPIXEL_PORT GPIO2
#define NEOPIXEL_PIN 5

#define NS_PER_SEC (1000000000L)
#define NS_PER_CYCLE(CYCLES_PER_SEC) (NS_PER_SEC / CYCLES_PER_SEC)
#define NS_TO_CYCLES(CYCLES_PER_SEC, n) ((n) / NS_PER_CYCLE(CYCLES_PER_SEC))

struct Timings {
  const uint32_t interval;
  const uint32_t bit_on;
  const uint32_t bit_off;
};

// Specific timings for WS2812
#define T1 300
#define T2 600
#define T3 300

// We need a macro to invoke every frame, since timings
// depend on SystemCoreClock, which is variable.
#define GET_TIMINGS(CYCLES_PER_SEC)                                            \
  Timings{.interval = NS_TO_CYCLES(CYCLES_PER_SEC, T1 + T2 + T3),              \
          .bit_on = NS_TO_CYCLES(CYCLES_PER_SEC, T2 + T3),                     \
          .bit_off = NS_TO_CYCLES(CYCLES_PER_SEC, T3)};

static void no_interrupts() {
  __disable_irq();
  DisableIRQ(DMA0_IRQn);
}

static void yes_interrupts() {
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
}

static inline void send_byte(uint8_t byte, uint32_t &next_cycle_start,
                             const Timings &timings) {

  // Read bits from highest to lowest by using a bitmask
  // which we shift for each bit.
  // Starting at 0x80 will give us 8 bits and when it is 0 we are done.

  uint32_t mask = 0x80;
  while (mask) {
    const uint8_t bit = byte & mask;

    // Wait for next interval cutoff
    while (DWT->CYCCNT < next_cycle_start)
      ;

    // Set next interval cutoff.
    // It is important that this happens immediately after the previous wait.
    next_cycle_start = DWT->CYCCNT + timings.interval;

    // Keep bit on for relevant time.
    const uint32_t on_cycles = bit ? timings.bit_on : timings.bit_off;
    const uint32_t on_cutoff =
        next_cycle_start - (timings.interval - on_cycles);
    pin_hi();
    while (DWT->CYCCNT < on_cutoff)
      ;

    // Pin will be kept low until next time send_byte is called.
    pin_lo();

    // Shift mask to reach next bit.
    mask >>= 1;
  }
}

static uint32_t show_pixels(const Pixel *const pixels, const int pixel_count) {
  // Enable DWT in debug core. Useable when interrupts disabled, as opposed to
  // Systick->VAL
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  const Pixel *const end = pixels + pixel_count;
  const Pixel *pixel_ptr = pixels;

  const auto freq = SystemCoreClock;
  const auto timings = GET_TIMINGS(freq);

#ifdef ALLOW_INTERRUPTS
  const uint32_t wait_off =
      NS_TO_CYCLES(freq, (WAIT_MICROSECONDS - INTERRUPT_THRESHOLD) * 1000);
#endif

  DWT->CYCCNT = 0;

  no_interrupts();
  pin_lo();

  uint32_t next_cycle_start = DWT->CYCCNT + timings.interval;

  while (pixel_ptr != end) {
#ifdef ALLOW_INTERRUPTS
    no_interrupts();

    if (DWT->CYCCNT > next_cycle_start) {
      if ((DWT->CYCCNT - next_cycle_start) > wait_off) {
        return false;
      }
    }
#endif

    const Pixel pix = *(pixel_ptr++);

    send_byte(pix.g, next_cycle_start, timings);
    send_byte(pix.r, next_cycle_start, timings);
    send_byte(pix.b, next_cycle_start, timings);

#ifdef ALLOW_INTERRUPTS
    yes_interrupts();
#endif
  }

  pin_lo();
#ifndef ALLOW_INTERRUPTS
  yes_interrupts();
#endif
  return true;
}

template <int WAIT_MICROS> class CMinWait {
  uint32_t mLastMicros;

public:
  CMinWait() { mLastMicros = 0; }

  void wait() {
    uint16_t diff;
    do {
      diff = (micros() & 0xFFFF) - mLastMicros;
    } while (diff < WAIT_MICROS);
  }

  void mark() { mLastMicros = micros() & 0xFFFF; }
};

CMinWait<WAIT_MICROSECONDS> mWait;

void show(const Pixel *const pixels, const int pixel_count) {
  pin_lo();
  mWait.wait();
  if (!show_pixels(pixels, pixel_count)) {
    yes_interrupts();
    delayMicroseconds(WAIT_MICROSECONDS);
    no_interrupts();
    show_pixels(pixels, pixel_count);
  }
  mWait.mark();
}

} // namespace LEDs
