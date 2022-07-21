#include "variant.h"
#include "fsl_lpuart.h"
/*
 * Pins descriptions
 */
const PinDescription g_APinDescription[] =
{
   {   //D0 GPIO_09 J56 #2
      .GROUP = GPIO1,
      .PIN = 9,
      .FUN_GPIO = {
          IOMUXC_GPIO_09_GPIOMUX_IO09
      }
  },
{   //D1 GPIO_10 J56 #4
      .GROUP = GPIO1,
      .PIN = 10,
      .FUN_GPIO = {
          IOMUXC_GPIO_10_GPIOMUX_IO10
      }
  },
{   //D2 GPIO_AD_05 J56 #6
      .GROUP = GPIO1,
      .PIN = 19,
      .ADC = ADC1,
      .adcChannel = 5,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_05_GPIOMUX_IO19
      }
  },
  {   //D3 GPIO_AD_06 J56 #8
      .GROUP = GPIO1,
      .PIN = 20,
      .ADC = ADC1,
      .adcChannel = 6,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_06_GPIOMUX_IO20
      }
  },
  {   //D4 GPIO_8 J56 #10 SAI1_MCLK on EVK1010
      .GROUP = GPIO1,
      .PIN = 8,
      .FUN_GPIO = {
          IOMUXC_GPIO_08_GPIOMUX_IO08
      },
      .FUN_SAI = {
          IOMUXC_GPIO_08_SAI1_MCLK
      }
  },
  {   //D5 GPIO_09 J56 #12 SAI1_RX_BCLK
      .GROUP = GPIO1,
      .PIN = 1,
      .FUN_GPIO = {
          IOMUXC_GPIO_01_GPIOMUX_IO01
      }
  },
  {   //D6 GPIO_AD_01 J56 #14
      .GROUP = GPIO1,
      .PIN = 15,
      .ADC = ADC1,
      .adcChannel = 1,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_01_GPIOMUX_IO15
      }
  },
  {   //D7 GPIO_AD_02 J56 #16
      .GROUP = GPIO1,
      .PIN = 16,
      .ADC = ADC1,
      .adcChannel = 2,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_02_GPIOMUX_IO16
      }
  },

  {   //D8 GPIO_SD_02 J57 #2
      .GROUP = GPIO2,
      .PIN = 2,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_02_GPIO2_IO02
      }
  },
  {   //D9 GPIO_03 J57 #4
      .GROUP = GPIO1,
      .PIN = 3,
      .FUN_GPIO = {
          IOMUXC_GPIO_03_GPIOMUX_IO03
      }
  },
  {   //D10 GPIO_AD_05 #6 LPSPI_PSC0
      .GROUP = GPIO1,
      .PIN = 19,
      .ADC = ADC1,
      .adcChannel = 5,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_05_GPIOMUX_IO19
      }
  },
  {   //D11 GPIO_AD_04 J57 #8 
      .GROUP = GPIO1,
      .PIN = 18,
      .ADC = ADC1,
      .adcChannel = 4,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_04_GPIOMUX_IO18
      }
  },
    {   //D12 GPIO_AD_03 J57 #10
      .GROUP = GPIO1,
      .PIN = 17,
      .ADC = ADC1,
      .adcChannel = 3,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_03_GPIOMUX_IO17
      }
  },
  {   //D13 J57 #12 GPIO_AD_06
      .GROUP = GPIO1,
      .PIN = 20,
      .ADC = ADC1,
      .adcChannel = 6,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_06_GPIOMUX_IO20
      }
  },
  {   //D14 J57 #18 GPIO_01
      .GROUP = GPIO1,
      .PIN = 1,
      .FUN_GPIO = {
          IOMUXC_GPIO_01_GPIOMUX_IO01
      }
  },
  {   //D15 J57 #20 GPIO_02
      .GROUP = GPIO1,
      .PIN = 2,
      .FUN_GPIO = {
          IOMUXC_GPIO_02_GPIOMUX_IO02
      }
  },
  {   //A0 J26 #2 GPIO_AD_07
      .GROUP = GPIO1,
      .PIN = 21,
      .ADC = ADC1,
      .adcChannel = 7,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_07_GPIOMUX_IO21
      }
  },
   {   //A1 J26 #4  GPIO_AD_09
      .GROUP = GPIO1,
      .PIN = 23,
      .ADC = ADC1,
      .adcChannel = 9,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_09_GPIOMUX_IO23
      }
  },
  {   //A2 J26 #6  GPIO_AD_10
      .GROUP = GPIO1,
      .PIN = 24,
      .ADC = ADC1,
      .adcChannel = 10,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_10_GPIOMUX_IO24
      }
  },
  {   //A3 J26 #8  GPIO_AD_14
      .GROUP = GPIO1,
      .PIN = 28,
      .ADC = ADC1,
      .adcChannel = 14,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_14_GPIOMUX_IO28
      }
  },
  {   //A4 J26 #10  GPIO_AD_01
      .GROUP = GPIO1,
      .PIN = 15,
      .ADC = ADC1,
      .adcChannel = 1,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_01_GPIOMUX_IO15
      }
  },
  {   //A5 J26 #12  GPIO_AD_02
      .GROUP = GPIO1,
      .PIN = 16,
      .ADC = ADC1,
      .adcChannel = 2,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_02_GPIOMUX_IO16
      }
  },
  {   //USER_LED GPIO_11
      .GROUP = GPIO1,
      .PIN = 11,
      .FUN_GPIO = {
          IOMUXC_GPIO_11_GPIOMUX_IO11
      }
  },
};
//     { MP_OBJ_NEW_QSTR(MP_QSTR_RX), MP_ROM_PTR(&pin_GPIO_09) },
//     { MP_OBJ_NEW_QSTR(MP_QSTR_TX), MP_ROM_PTR(&pin_GPIO_10) },

//     { MP_OBJ_NEW_QSTR(MP_QSTR_MOSI), MP_ROM_PTR(&pin_GPIO_AD_04) },
//     { MP_OBJ_NEW_QSTR(MP_QSTR_MISO), MP_ROM_PTR(&pin_GPIO_AD_03) },
//     { MP_OBJ_NEW_QSTR(MP_QSTR_SCK), MP_ROM_PTR(&pin_GPIO_AD_06) },

//     { MP_OBJ_NEW_QSTR(MP_QSTR_SDA), MP_ROM_PTR(&pin_GPIO_01) },
//     { MP_OBJ_NEW_QSTR(MP_QSTR_SCL), MP_ROM_PTR(&pin_GPIO_02) },

//     { MP_OBJ_NEW_QSTR(MP_QSTR_USER_LED), MP_ROM_PTR(&pin_GPIO_11) },
//     { MP_OBJ_NEW_QSTR(MP_QSTR_LED), MP_ROM_PTR(&pin_GPIO_11) },

//     { MP_OBJ_NEW_QSTR(MP_QSTR_USER_SW), MP_ROM_PTR(&pin_GPIO_SD_05) },

//     // Audio Interface
//     { MP_ROM_QSTR(MP_QSTR_AUDIO_INT), MP_ROM_PTR(&pin_GPIO_00) },
//     { MP_ROM_QSTR(MP_QSTR_AUDIO_SYNC), MP_ROM_PTR(&pin_GPIO_07) },
//     { MP_ROM_QSTR(MP_QSTR_AUDIO_BCLK), MP_ROM_PTR(&pin_GPIO_06) },
//     { MP_ROM_QSTR(MP_QSTR_AUDIO_RXD), MP_ROM_PTR(&pin_GPIO_03) },
//     { MP_ROM_QSTR(MP_QSTR_AUDIO_TXD), MP_ROM_PTR(&pin_GPIO_04) },
//     { MP_ROM_QSTR(MP_QSTR_AUDIO_MCLK), MP_ROM_PTR(&pin_GPIO_08) },

//     // SPDIF
//     { MP_ROM_QSTR(MP_QSTR_SPDIF_IN), MP_ROM_PTR(&pin_GPIO_10) },
//     { MP_ROM_QSTR(MP_QSTR_SPDIF_OUT), MP_ROM_PTR(&pin_GPIO_11) },

//     // Freelink UART
//     { MP_ROM_QSTR(MP_QSTR_FREELINK_TX), MP_ROM_PTR(&pin_GPIO_10) },
//     { MP_ROM_QSTR(MP_QSTR_FREELINK_RX), MP_ROM_PTR(&pin_GPIO_09) },

//     { MP_ROM_QSTR(MP_QSTR_I2C), MP_ROM_PTR(&board_i2c_obj) },
//     { MP_ROM_QSTR(MP_QSTR_SPI), MP_ROM_PTR(&board_spi_obj) },
//     { MP_ROM_QSTR(MP_QSTR_UART), MP_ROM_PTR(&board_uart_obj) },