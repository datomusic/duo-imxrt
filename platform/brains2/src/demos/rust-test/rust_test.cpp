#include "Arduino.h"
#include "flexio_configuration.h"
#include "fsl_flexio.h"
#include "lib/board_init.h"

#define LED2_PINMUX IOMUXC_GPIO_07_GPIOMUX_IO07
#define LED2_PORT GPIO1
#define LED2_PIN 7U
#define NEOPIXEL_PINMUX IOMUXC_GPIO_SD_05_FLEXIO1_IO11

extern "C" {
void rust_main();

void delay_mic(uint32_t mics) { delayMicroseconds(mics); };
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

  uint8_t shifter_output_start_pin = 0;
  uint8_t shift_timer_output_pin = shifter_output_start_pin + 4;

  uint8_t data_shifter = 0;
  uint8_t shifter_timer = 0;
  uint8_t idle_timer = 1;
  uint8_t pin_pos = 0;
  uint8_t low_bit_timer = 2;
  uint8_t high_bit_timer = 3;

  uint8_t neopixel_output_pin = 11;

  // Reset
  FLEXIO1->CTRL = FLEXIO_CTRL_SWRST(1);
  FLEXIO1->CTRL = FLEXIO_CTRL_SWRST(0);

  // init_neopixel();

  configure_shifter(data_shifter, shifter_timer, shifter_output_start_pin);
  configure_shift_timer(data_shifter, shifter_timer, shift_timer_output_pin);
  configure_idle_timer(idle_timer, shift_timer_output_pin);

  configure_low_bit_timer(low_bit_timer, shift_timer_output_pin,
                          neopixel_output_pin);
  configure_high_bit_timer(high_bit_timer, shifter_output_start_pin + pin_pos,
                           neopixel_output_pin);

  IOMUXC_SetPinMux(NEOPIXEL_PINMUX, 0U);

  // Start it
  FLEXIO1->CTRL = FLEXIO_CTRL_FLEXEN(1);

  for (;;) {
    write_data();
    delayMicroseconds(1000);
  }

  return 0;
}
