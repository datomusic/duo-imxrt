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
      },
      .FUN_UART = {
        IOMUXC_GPIO_09_LPUART1_RXD
      }
  },
{   //D1 GPIO_10 J56 #4
      .GROUP = GPIO1,
      .PIN = 10,
      .FUN_GPIO = {
          IOMUXC_GPIO_10_GPIOMUX_IO10
      },
      .FUN_UART = {
        IOMUXC_GPIO_10_LPUART1_TXD
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
  {   // 23U AD_11
      .GROUP = GPIO1,
      .PIN = 25,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_11_GPIOMUX_IO25
      }
  },
};