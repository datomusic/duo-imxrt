#include "lib/variant.h"
extern "C" {
#include "fsl_gpio.h"
}

#define HP_ENABLE_PINMUX IOMUXC_GPIO_AD_11_GPIOMUX_IO25
#define HP_ENABLE_PORT GPIO1
#define HP_ENABLE_PIN 25U

#define AMP_MUTE_PINMUX IOMUXC_GPIO_AD_10_GPIOMUX_IO24
#define AMP_MUTE_PORT GPIO1
#define AMP_MUTE_PIN 24U

namespace Audio {
bool amp_enable_polarity = false;

void headphone_enable(void) {
  IOMUXC_SetPinMux(HP_ENABLE_PINMUX, 0U);
  gpio_pin_config_t hp_enable_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(HP_ENABLE_PORT, HP_ENABLE_PIN, &hp_enable_config);
  GPIO_PinWrite(HP_ENABLE_PORT, HP_ENABLE_PIN, 1);
}

void headphone_disable(void) {
  IOMUXC_SetPinMux(HP_ENABLE_PINMUX, 0U);
  gpio_pin_config_t hp_enable_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(HP_ENABLE_PORT, HP_ENABLE_PIN, &hp_enable_config);
  GPIO_PinWrite(HP_ENABLE_PORT, HP_ENABLE_PIN, 0);
}

void amp_enable(void) {
  GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, (amp_enable_polarity ? 1 : 0));
}

void amp_disable(void) {
  GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, (amp_enable_polarity ? 0 : 1));
}

void amp_init(void) {
  // BIT 12 should be 0 to disable pull/keeper
  const uint32_t IOMUXC_PINCONFIG_DEFAULT = 0xB0U;

  IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
  IOMUXC_SetPinConfig(AMP_MUTE_PINMUX, IOMUXC_PINCONFIG_DEFAULT);
  gpio_pin_config_t amp_mute_config = {kGPIO_DigitalInput, 0};
  GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);

  // An external resistor sets the amp to mute at startup. 
  // Some chips are active high others are active low
  // Read its direction to determine the polarity
  amp_enable_polarity = !(GPIO_PinRead(AMP_MUTE_PORT, AMP_MUTE_PIN));
  
  IOMUXC_SetPinConfig(AMP_MUTE_PINMUX, (1<<12) | IOMUXC_PINCONFIG_DEFAULT);
  // Reconfigure as output with initial state
  amp_mute_config.direction = kGPIO_DigitalOutput;
  GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
  GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, (amp_enable_polarity ? 0 : 1));
}

} // namespace Audio