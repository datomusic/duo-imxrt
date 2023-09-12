#include "Arduino.h"
#include "fsl_flexio.h"
#include "lib/board_init.h"

#define LED2_PINMUX IOMUXC_GPIO_07_GPIOMUX_IO07
#define LED2_PORT GPIO1
#define LED2_PIN 7U

extern "C" {
void rust_main();

void delay_mic(uint32_t mics) { delayMicroseconds(mics); };
}

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

int main(void) {
  board_init();

  init_debug_led();

  // flash_led(100);
  // delayMicroseconds(1000 * 500);
  // flash_led(1000);
  // delayMicroseconds(1000 * 2000);


  init_flexio();
  rust_main();
  return 0;
}
