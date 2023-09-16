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

uint8_t subcounter = 0;
uint8_t counter = 0;
extern "C" void write_data() {

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
}

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
  rust_main();
  return 0;
}
