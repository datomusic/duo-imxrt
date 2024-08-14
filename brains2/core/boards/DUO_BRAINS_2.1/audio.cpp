#include "lib/variant.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define HP_ENABLE_PINMUX IOMUXC_GPIO_AD_11_GPIOMUX_IO25
#define HP_ENABLE_PORT GPIO1
#define HP_ENABLE_PIN 25U

#define AMP_MUTE_PINMUX IOMUXC_GPIO_AD_10_GPIOMUX_IO24
#define AMP_MUTE_PORT GPIO1
#define AMP_MUTE_PIN 24U

namespace Audio {
bool amp_active_high = false;
bool amp_enabled = false;

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

void amp_init(void) {
  IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
  IOMUXC_SetPinConfig(AMP_MUTE_PINMUX, 0x10B0U);
  gpio_pin_config_t amp_mute_config = {kGPIO_DigitalInput, kGPIO_NoIntmode};
  GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
  Audio::amp_active_high = (GPIO_PinRead(AMP_MUTE_PORT, AMP_MUTE_PIN) == 0);
}

void amp_enable(void) {
  if(!Audio::amp_enabled) {
    IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
    gpio_pin_config_t amp_mute_config = {kGPIO_DigitalOutput, 0};
    GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
    GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN, (Audio::amp_active_high ? 1 : 0));
    Audio::amp_enabled = true;
  }
}

void amp_disable(void) {
  if(Audio::amp_enabled) {
    IOMUXC_SetPinMux(AMP_MUTE_PINMUX, 0U);
    gpio_pin_config_t amp_mute_config = {kGPIO_DigitalOutput, 0};
    GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
    GPIO_PinWrite(AMP_MUTE_PORT, AMP_MUTE_PIN,  (Audio::amp_active_high ? 0 : 1));
    // Release the pin. The pullup/pulldown resistor will take care of the rest
    amp_mute_config = {kGPIO_DigitalInput, 0};
    GPIO_PinInit(AMP_MUTE_PORT, AMP_MUTE_PIN, &amp_mute_config);
    Audio::amp_enabled = false;
  }
}

} // namespace Audio
