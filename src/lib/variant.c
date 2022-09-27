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
  // TODO: add all pin mappings for DUO Brains
};
