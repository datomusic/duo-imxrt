#include "Arduino.h"
#include "flexio_led_driver.h"
#include "lib/board_init.h"

#define LED2_PINMUX IOMUXC_GPIO_07_GPIOMUX_IO07
#define LED2_PORT GPIO1
#define LED2_PIN 7U

void flash_led(const uint32_t ms) {
  IOMUXC_SetPinMux(LED2_PINMUX, 0U);
  gpio_pin_config_t led2_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);

  GPIO_PinWrite(LED2_PORT, LED2_PIN, 1);
  delayMicroseconds(1000 * ms);
  GPIO_PinWrite(LED2_PORT, LED2_PIN, 0);
}

void flash_led() { flash_led(100); }

void init_debug_led() {
  IOMUXC_SetPinMux(LED2_PINMUX, 0U);
  gpio_pin_config_t led2_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);
}

const unsigned PIXEL_COUNT = 19;

const unsigned BYTE_COUNT = 3 * PIXEL_COUNT;
uint8_t bytes[BYTE_COUNT];

int main(void) {
  board_init();

  init_debug_led();

  // flash_led(100);
  // delayMicroseconds(1000 * 500);
  // flash_led(1000);
  // delayMicroseconds(1000 * 2000);

  setup_flexio_leds();

  uint8_t counter = 0;

  for (;;) {

    for (unsigned i = 0; i < BYTE_COUNT; ++i) {
      bytes[i] = 0;
    }

    const uint8_t p = (counter % PIXEL_COUNT) * 3;
    bytes[p] = 255;
    bytes[p + 1] = 255;
    bytes[p + 2] = 255;

    show_bytes(bytes, BYTE_COUNT);

    ++counter;

    delayMicroseconds(100000);
  }

  return 0;
}
