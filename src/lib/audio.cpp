#include "lib/variant.h"

namespace Audio {
void headphone_enable(void) {
#define HP_ENABLE_PINMUX IOMUXC_GPIO_AD_11_GPIOMUX_IO25
#define HP_ENABLE_PORT GPIO1
#define HP_ENABLE_PIN 25U

  IOMUXC_SetPinMux(HP_ENABLE_PINMUX, 0U);
  gpio_pin_config_t hp_enable_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(HP_ENABLE_PORT, HP_ENABLE_PIN, &hp_enable_config);
  GPIO_PinWrite(HP_ENABLE_PORT, HP_ENABLE_PIN, 1);
}

void headphone_disable(void) {
#define HP_ENABLE_PINMUX IOMUXC_GPIO_AD_11_GPIOMUX_IO25
#define HP_ENABLE_PORT GPIO1
#define HP_ENABLE_PIN 25U

  IOMUXC_SetPinMux(HP_ENABLE_PINMUX, 0U);
  gpio_pin_config_t hp_enable_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(HP_ENABLE_PORT, HP_ENABLE_PIN, &hp_enable_config);
  GPIO_PinWrite(HP_ENABLE_PORT, HP_ENABLE_PIN, 0);
}

void amp_enable(void) {
#define AMP_MUTE_PINMUX IOMUXC_GPIO_AD_10_GPIOMUX_IO24
#define AMP_MUTE_PORT GPIO1
#define AMP_MUTE_PIN 24U

  IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
  gpio_pin_config_t amp_mute_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
  GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, 0);
}

void amp_disable(void) {
#define AMP_MUTE_PINMUX IOMUXC_GPIO_AD_10_GPIOMUX_IO24
#define AMP_MUTE_PORT GPIO1
#define AMP_MUTE_PIN 24U

  IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
  gpio_pin_config_t amp_mute_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
  GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, 1);
}

} // namespace Audio
