#include "Arduino.h" //for delayMicroseconds()
// #include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

// TODO: Switch to Arduino pin mappings once those are available
#define BS814A_CLOCK_PERIOD_US 20
#define BS814A_CLOCK_GPIO GPIO1
#define BS814A_CLOCK_PIN 3U
#define BS814A_CLOCK_MUX IOMUXC_GPIO_03_GPIOMUX_IO03
#define BS814A_DATA_GPIO GPIO1
#define BS814A_DATA_PIN 4U
#define BS814A_DATA_MUX IOMUXC_GPIO_04_GPIOMUX_IO04

void BS814A_begin(void);
uint8_t BS814A_readRaw(void);

void BS814A_begin()
{
  // Clock pin configuration
  IOMUXC_SetPinMux(BS814A_CLOCK_MUX, 0U);
  IOMUXC_SetPinConfig(BS814A_CLOCK_MUX , 0x10B0U);
  gpio_pin_config_t output_config = { kGPIO_DigitalOutput, 0, kGPIO_NoIntmode };
  GPIO_PinInit(BS814A_CLOCK_GPIO, BS814A_CLOCK_PIN, &output_config);
  GPIO_PinWrite(BS814A_CLOCK_GPIO, BS814A_CLOCK_PIN, 1);
  
  // Data pin configuration
  IOMUXC_SetPinMux(BS814A_DATA_MUX, 0U);
  IOMUXC_SetPinConfig(BS814A_DATA_MUX, 0x10B0U);
  gpio_pin_config_t input_config = { kGPIO_DigitalInput, 0, kGPIO_NoIntmode };
  GPIO_PinInit(BS814A_DATA_GPIO, BS814A_DATA_PIN, &input_config);
}

/* 
  Bit 0:3 are Key 1 to Key 4 state. 0 is touched, 1 is no touch 
  Bit 4:6 is a checksum. The number of keys touched
  Bit 7 is a stop bit. It is always 1

  The BS814A pulls the data pin low to signal that a touch is detected
*/
uint8_t BS814A_readRaw() 
{
  if (GPIO_PinRead(BS814A_DATA_GPIO, BS814A_DATA_PIN) == 1)
  {
    return 0;
  }

  uint8_t value = 0;
  uint8_t mask;
  for (mask=0x01; mask; mask <<= 1) // LSBFIRST
  { 
    // digitalWrite(BS814A_CLOCK_PIN, LOW);
    GPIO_PinWrite(BS814A_CLOCK_GPIO, BS814A_CLOCK_PIN, 0);

    delayMicroseconds(BS814A_CLOCK_PERIOD_US);

    if (GPIO_PinRead(BS814A_DATA_GPIO, BS814A_DATA_PIN)) value |= mask;

    // digitalWrite(BS814A_CLOCK_PIN, HIGH);
    GPIO_PinWrite(BS814A_CLOCK_GPIO, BS814A_CLOCK_PIN, 1);

    delayMicroseconds(BS814A_CLOCK_PERIOD_US);
  }
  return value; 
}
