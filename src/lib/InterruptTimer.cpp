#include "Arduino.h"
#include "InterruptTimer.h"
#include "fsl_pit.h"
#include "fsl_clock.h"
#include "clock_config.h"
#include "fsl_common.h"

#define PIT_CHANNEL  kPIT_Chnl_0

namespace InterruptTimer {
  void init() {
    pit_config_t pitConfig;
    /* Set PERCLK_CLK source to OSC_CLK*/
    CLOCK_SetMux(kCLOCK_PerclkMux, 1U);
    /* Set PERCLK_CLK divider to 1 */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0U);

    PIT_GetDefaultConfig(&pitConfig);

    PIT_Init(PIT, &pitConfig);

    PIT_SetTimerPeriod(PIT, PIT_CHANNEL, USEC_TO_COUNT(100'000U, CLOCK_GetFreq(kCLOCK_OscClk)));

    PIT_EnableInterrupts(PIT, PIT_CHANNEL, kPIT_TimerInterruptEnable);

    NVIC_SetVector(PIT_IRQn, (uint32_t)&InterruptTimer::isr);
    NVIC_SetPriority(PIT_IRQn, 32);
    EnableIRQ(PIT_IRQn);

    PIT_StartTimer(PIT, PIT_CHANNEL);
  }

  void setTimerPeriod(uint32_t usec) {
    PIT_SetTimerPeriod(PIT, PIT_CHANNEL, USEC_TO_COUNT(usec, CLOCK_GetFreq(kCLOCK_OscClk))); 
  }

  void isr(void) {
    /* Clear interrupt flag.*/
    if(PIT_GetStatusFlags(PIT, PIT_CHANNEL)) {
      digitalWrite(GPIO_AD_03, HIGH);

      internal_clock++;
      /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
      * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
      * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
      */
      digitalWrite(GPIO_AD_03, LOW);
    }
    PIT_ClearStatusFlags(PIT, PIT_CHANNEL, kPIT_TimerFlag);
  
    __DSB();
  }
}
