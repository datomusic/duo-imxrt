#include "variant.h"
#include "fsl_lpuart.h"
/*
 * Pins descriptions for DUO Brains 2
 */
const PinDescription g_APinDescription[] =
{
   {  
      .GROUP = GPIO1,
      .PIN = 0,
      .FUN_GPIO = {
          IOMUXC_GPIO_00_GPIOMUX_IO00
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 1,
      .FUN_GPIO = {
          IOMUXC_GPIO_01_GPIOMUX_IO01
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_0,
      .pwm_channel = kPWM_PwmB,
      .FUN_PWM = {
        IOMUXC_GPIO_01_FLEXPWM1_PWM0_B
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 2,
      .FUN_GPIO = {
          IOMUXC_GPIO_02_GPIOMUX_IO02
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_0,
      .pwm_channel = kPWM_PwmA,
      .FUN_PWM = {
          IOMUXC_GPIO_02_FLEXPWM1_PWM0_A
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 3,
      .FUN_GPIO = {
          IOMUXC_GPIO_03_GPIOMUX_IO03
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_0,
      .pwm_channel = kPWM_PwmB,
      .FUN_PWM = {
          IOMUXC_GPIO_03_FLEXPWM1_PWM1_B
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 4,
      .FUN_GPIO = {
          IOMUXC_GPIO_04_GPIOMUX_IO04
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_1,
      .pwm_channel = kPWM_PwmA,
      .FUN_PWM = {
          IOMUXC_GPIO_04_FLEXPWM1_PWM1_A
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 5,
      .FUN_GPIO = {
          IOMUXC_GPIO_05_GPIOMUX_IO05
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_2,
      .pwm_channel = kPWM_PwmB,
      .FUN_PWM = {
          IOMUXC_GPIO_05_FLEXPWM1_PWM2_B
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 6,
      .FUN_GPIO = {
          IOMUXC_GPIO_06_GPIOMUX_IO06
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_2,
      .pwm_channel = kPWM_PwmA,
      .FUN_PWM = {
          IOMUXC_GPIO_06_FLEXPWM1_PWM2_A
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 7,
      .FUN_GPIO = {
          IOMUXC_GPIO_07_GPIOMUX_IO07
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_3,
      .pwm_channel = kPWM_PwmB,
      .FUN_PWM = {
          IOMUXC_GPIO_07_FLEXPWM1_PWM3_B
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 8,
      .FUN_GPIO = {
          IOMUXC_GPIO_08_GPIOMUX_IO08
      },
      .PWM = PWM1,
      .pwm_submodule = kPWM_Module_3,
      .pwm_channel = kPWM_PwmA,
      .FUN_PWM = {
          IOMUXC_GPIO_08_FLEXPWM1_PWM3_A
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 9,
      .FUN_GPIO = {
          IOMUXC_GPIO_09_GPIOMUX_IO09
      },
      .FUN_UART = {
          IOMUXC_GPIO_09_LPUART1_RXD
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 10,
      .FUN_GPIO = {
          IOMUXC_GPIO_10_GPIOMUX_IO10
      },
      .FUN_UART = {
          IOMUXC_GPIO_10_LPUART1_TXD
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 11,
      .FUN_GPIO = {
          IOMUXC_GPIO_11_GPIOMUX_IO11
      }
  },
   {  
      .GROUP = GPIO1,
      .PIN = 12,
      .FUN_GPIO = {
          IOMUXC_GPIO_12_GPIOMUX_IO12
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 13,
      .FUN_GPIO = {
          IOMUXC_GPIO_13_GPIOMUX_IO13
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 14,
      .ADC = ADC1,
      .adcChannel = 0,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_00_GPIOMUX_IO14
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 15,
      .ADC = ADC1,
      .adcChannel = 1,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_01_GPIOMUX_IO15
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 16,
      .ADC = ADC1,
      .adcChannel = 2,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_02_GPIOMUX_IO16
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 17,
      .ADC = ADC1,
      .adcChannel = 3,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_03_GPIOMUX_IO17
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 18,
      .ADC = ADC1,
      .adcChannel = 4,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_04_GPIOMUX_IO18
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 19,
      .ADC = ADC1,
      .adcChannel = 5,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_05_GPIOMUX_IO19
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 20,
      .ADC = ADC1,
      .adcChannel = 6,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_06_GPIOMUX_IO20
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 21,
      .ADC = ADC1,
      .adcChannel = 7,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_07_GPIOMUX_IO21
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 22,
      .ADC = ADC1,
      .adcChannel = 8,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_08_GPIOMUX_IO22
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 23,
      .ADC = ADC1,
      .adcChannel = 9,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_09_GPIOMUX_IO23
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 24,
      .ADC = ADC1,
      .adcChannel = 10,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_10_GPIOMUX_IO24
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 25,
      .ADC = ADC1,
      .adcChannel = 11,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_11_GPIOMUX_IO25
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 26,
      .ADC = ADC1,
      .adcChannel = 12,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_12_GPIOMUX_IO26
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 27,
      .ADC = ADC1,
      .adcChannel = 13,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_13_GPIOMUX_IO27
      }
  },
    {  
      .GROUP = GPIO1,
      .PIN = 28,
      .ADC = ADC1,
      .adcChannel = 14,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_14_GPIOMUX_IO28
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 0,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_00_GPIO2_IO00
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 1,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_01_GPIO2_IO01
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 2,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_02_GPIO2_IO02
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 3,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_03_GPIO2_IO03
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 4,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_04_GPIO2_IO04
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 5,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_05_GPIO2_IO05
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 6,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_06_GPIO2_IO06
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 7,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_07_GPIO2_IO07
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 8,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_08_GPIO2_IO08
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 9,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_09_GPIO2_IO09
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 10,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_10_GPIO2_IO10
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 11,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_11_GPIO2_IO11
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 12,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_12_GPIO2_IO12
      }
  },
    {  
      .GROUP = GPIO2,
      .PIN = 13,
      .FUN_GPIO = {
          IOMUXC_GPIO_SD_13_GPIO2_IO13
      }
  },
};
