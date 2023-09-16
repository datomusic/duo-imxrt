#include "Arduino.h"
#include "flexio_configuration.h"
#include "fsl_flexio.h"
#include "lib/board_init.h"

#define LED2_PINMUX IOMUXC_GPIO_07_GPIOMUX_IO07
#define LED2_PORT GPIO1
#define LED2_PIN 7U

extern "C" {
void rust_main();
void show_pixels(uint32_t size, const uint8_t *bytes);

void delay_mic(uint32_t mics) { delayMicroseconds(mics); };

void configure_shifter(const uint8_t shifter_id, const uint8_t input_timer,
                       const uint8_t output_start_pin) {

  FLEXIO1->SHIFTCTL[shifter_id] =
      FLEXIO_SHIFTCTL_TIMSEL(input_timer) |
      FLEXIO_SHIFTCTL_TIMPOL(0) | // shift on posedge of shift clock
      FLEXIO_SHIFTCTL_PINCFG(3) | // shifter_id pin output
      FLEXIO_SHIFTCTL_PINSEL(output_start_pin) |
      FLEXIO_SHIFTCTL_PINPOL(0) | // pin is active high
      FLEXIO_SHIFTCTL_SMOD(2);    // shifter_id mode: transmitter

  FLEXIO1->SHIFTCFG[shifter_id] =
      FLEXIO_SHIFTCFG_PWIDTH(3) | // NA
      FLEXIO_SHIFTCFG_INSRC(0) |  // NA
      FLEXIO_SHIFTCFG_SSTOP(0) |  // STOP bit disabled
      FLEXIO_SHIFTCFG_SSTART(1); // START bit disabled; load data on first shift
}

void configure_shift_timer(const uint8_t timer_id, const uint8_t shifter_id,
                           const uint8_t output_pin) {
  const uint32_t CYCLES_PER_SHIFTBUFFER = 16;

  FLEXIO1->TIMCMP[timer_id] =
      ((CYCLES_PER_SHIFTBUFFER - 1) << 8) | (CLOCK_DIVIDER - 1);

  FLEXIO1->TIMCTL[timer_id] =
      FLEXIO_TIMCTL_TRGSEL((uint32_t)(shifter_id * 4) + 1) |
      FLEXIO_TIMCTL_TRGSRC(1) | // internal trigger
      FLEXIO_TIMCTL_PINCFG(3) | // timer pin output
      FLEXIO_TIMCTL_PINPOL(0) | // output active high
      FLEXIO_TIMCTL_TIMOD(1) |  // dual 8-bit counter baud/bit mode
      FLEXIO_TIMCTL_PINSEL((uint32_t)output_pin) |
      FLEXIO_TIMCTL_TRGPOL(1); // trigger active low

  FLEXIO1->TIMCFG[timer_id] =
      FLEXIO_TIMCFG_TIMOUT(1) | // Zero when enabled, not affected by reset
      FLEXIO_TIMCFG_TIMDEC(0) | // Input clock from FlexIO clock
      FLEXIO_TIMCFG_TIMRST(0) | // never reset
      FLEXIO_TIMCFG_TIMDIS(2) | // disable on timer compare
      FLEXIO_TIMCFG_TIMENA(2) | // enabled on trigger high
      FLEXIO_TIMCFG_TSTOP(0) |  // STOP bit disabled
      FLEXIO_TIMCFG_TSTART(0);  // START bit disabled
}
}

const unsigned PIXEL_COUNT = 19;
const unsigned BYTE_COUNT = 4 * 3 * PIXEL_COUNT;
uint8_t bytes[BYTE_COUNT + 1];

void flash_led(const uint32_t ms) {
  IOMUXC_SetPinMux(LED2_PINMUX, 0U);
  gpio_pin_config_t led2_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);

  GPIO_PinWrite(LED2_PORT, LED2_PIN, 1);
  delayMicroseconds(1000 * ms);
  GPIO_PinWrite(LED2_PORT, LED2_PIN, 0);
}

extern "C" void flash_led() { flash_led(100); }

void init_debug_led() {
  IOMUXC_SetPinMux(LED2_PINMUX, 0U);
  gpio_pin_config_t led2_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);
}

void init_flexio() {
  flexio_config_t fxioUserConfig;
  FLEXIO_GetDefaultConfig(&fxioUserConfig);
  FLEXIO_Init(FLEXIO1, &fxioUserConfig);
}

bool shift_buffer_empty() {
  const uint32_t shifter_id = 0;
  const uint32_t mask = 1 << shifter_id;
  return (FLEXIO1->SHIFTSTAT & mask) != 0;
}

uint32_t spread4(uint8_t _x) {
  uint32_t x = _x;

  x = (x | (x << 12)) & 0x000F000F;
  x = (x | (x << 6)) & 0x03030303;
  x = (x | (x << 3)) & 0x11111111;

  return x;
}
void reset_idle_timer_finished_flag() {
  const uint32_t idle_timer_id = 1;
  const uint32_t mask = 1 << idle_timer_id;
  FLEXIO1->TIMSTAT = mask;
}

bool idle_timer_finished() {
  const uint32_t idle_timer_id = 1;
  const uint32_t mask = 1 << idle_timer_id;

  return (FLEXIO1->TIMSTAT & mask) != 0;
}

uint8_t subcounter = 0;
uint8_t counter = 0;
extern "C" void write_data() {
  while (!shift_buffer_empty()) {
  }
  reset_idle_timer_finished_flag();

  if (++subcounter > 3) {
    subcounter = 0;
    ++counter;
  }

  for (unsigned i = 0; i < BYTE_COUNT; ++i) {
    bytes[i] = (counter - i * 3);
  }

  const unsigned buf_id = 0;
  uint32_t next_data = 0;
  uint32_t pos = 0;
  for (unsigned byte_index = 0; byte_index < BYTE_COUNT; ++byte_index) {
    next_data |= spread4(bytes[byte_index]) << (3 - pos);

    ++pos;
    if (pos == 4) {
      FLEXIO1->SHIFTBUFBIS[buf_id] = next_data;
      while (!shift_buffer_empty()) {
      }
      pos = 0;
      next_data = 0;
    }
  }

  while (!idle_timer_finished()) {
  }
}

extern "C" void init_neopixel();

int main(void) {
  board_init();

  init_debug_led();

  // flash_led(100);
  // delayMicroseconds(1000 * 500);
  // flash_led(1000);
  // delayMicroseconds(1000 * 2000);

  init_flexio();
  configure_flexio_clock();

  bytes[0] = 0;
  bytes[4] = 0;
  bytes[5] = 255;
  bytes[6] = 255;
  bytes[7] = 255;
  bytes[8] = 255;

  // bytes[12] = 0;

  // bytes[2] = 0;
  // bytes[3] = 0;
  // bytes[4] = 200;
  // bytes[5] = 200;
  // bytes[6] = 200;
  // bytes[7] = 200;
  show_pixels(BYTE_COUNT, bytes);
  init_neopixel();
  for (;;) {
    write_data();
    delayMicroseconds(1000);
  }

  return 0;
}
