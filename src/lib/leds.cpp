#include "leds.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include <Arduino.h>
#include <Audio.h>
#include "teensy_audio_config.h"

#define NEOPIXEL_PINMUX IOMUXC_GPIO_SD_05_GPIO2_IO05
#define NEOPIXEL_PORT GPIO2
#define NEOPIXEL_PIN 5


static void no_interrupts(){
  AudioNoInterrupts();
  __disable_irq();
}

static void yes_interrupts(){
  AudioInterrupts();
  __enable_irq();
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

static inline void send_bit(uint8_t bit, uint32_t &next_mark, uint32_t *off) {
  while (DWT->CYCCNT < next_mark)
    ;

  const uint32_t on_cycles = bit ? off[2] : off[1];

  next_mark = DWT->CYCCNT + off[0];
  const uint32_t on_cutoff = next_mark - (off[0] - on_cycles);

  pin_hi();

  while (DWT->CYCCNT < on_cutoff)
    ;

  pin_lo();
}

static inline uint32_t send_byte(uint8_t byte, uint32_t &next_mark,
                                 uint32_t *off) {
  uint32_t mask = 0x80;

  while (mask) {
    const uint8_t bit = byte & mask;
    send_bit(bit, next_mark, off);
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

#define WAIT_TIME 100

CMinWait<WAIT_TIME> mWait;

static uint32_t show_pixels(const Pixel *const pixels, const int pixel_count) {
  const uint32_t start = DWT->CYCCNT;

  // assumes 800_000Hz frequency
  // Theoretical values here are 800_000 -> 1.25us, 2500000->0.4us,
  // 1250000->0.8us
  // TODO: try to get dynamic weighting working again

  const uint8_t *const pixel_bytes = (uint8_t *)pixels;
  const uint8_t *byte_ptr = pixel_bytes;
  const uint32_t byte_count = pixel_count * 3;
  const uint8_t *const end = pixel_bytes + byte_count;

  /* const uint32_t last = DWT->CYCCNT; */

#define NS_PER_SEC                                                             \
  (1000000000L) // Note that this has to be SIGNED since we want to be able to
                // check for negative values of derivatives
#define CYCLES_PER_SEC (SystemCoreClock)
#define NS_PER_CYCLE (NS_PER_SEC / CYCLES_PER_SEC)
#define NS_TO_CYCLES(n) ((n) / NS_PER_CYCLE)

#define MAGIC_800_INT 900'000   // ~1.11 us -> 1.2  field
#define MAGIC_800_T0H 2'800'000 // ~0.36 us -> 0.44 field
#define MAGIC_800_T1H 1'350'000 // ~0.74 us -> 0.84 field
#define MAGIC_800_RST 4000      // 80 us reset time

#define interval (SystemCoreClock / MAGIC_800_INT)
#define t0 (SystemCoreClock / MAGIC_800_T0H)
#define t1 (SystemCoreClock / MAGIC_800_T1H)

  uint32_t off[3];
  off[0] = interval;
  off[1] = t0;
  off[2] = t1;

#define INTERRUPT_THRESHOLD 1

  const uint32_t wait_off =
      NS_TO_CYCLES((WAIT_TIME - INTERRUPT_THRESHOLD) * 1000);

  no_interrupts();
  pin_lo();

  uint32_t next_mark = DWT->CYCCNT + off[0];
  while (byte_ptr != end) {
    /* no_interrupts(); */

    /* if (next_mark > 0 && DWT->CYCCNT > next_mark) { */
    /*   if ((DWT->CYCCNT - next_mark) > wait_off) { */
    /*     yes_interrupts(); */
    /*     return DWT->CYCCNT - start; */
    /*   } */
    /* } */

    send_byte(*byte_ptr, next_mark, off);
    byte_ptr++;

    send_byte(*byte_ptr, next_mark, off);
    byte_ptr++;

    send_byte(*byte_ptr, next_mark, off);
    byte_ptr++;

    /* yes_interrupts(); */
  }

  pin_lo();

  const uint32_t rst = SystemCoreClock / MAGIC_800_RST;
  while ((DWT->CYCCNT - start) < rst)
    ;

  yes_interrupts();
  return DWT->CYCCNT - start;
}

void show(const Pixel *const pixels, const int pixel_count) {
  show_pixels(pixels, pixel_count);

  /* mWait.wait(); */
  /* if (!show_pixels(pixels, pixel_count)) { */
  /*   yes_interrupts(); */
  /*   delayMicroseconds(WAIT_TIME); */
  /*   no_interrupts(); */
  /*   show_pixels(pixels, pixel_count); */
  /* } */
  /* mWait.mark(); */
}

} // namespace LEDs
