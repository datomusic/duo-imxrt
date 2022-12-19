/**
 * The MIT License (MIT)
 * 
 * Author: Hongtai Liu (lht856@foxmail.com)
 * 
 * Copyright (C) 2019  Seeed Technology Co.,Ltd. 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Arduino.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_adc.h"
#include "fsl_common_arm.h"
#include "fsl_xbara.h"
#include "fsl_pwm.h"
#include "fsl_common.h"

volatile uint32_t systick_millis_count = 0;
volatile uint32_t systick_cycle_count = 0;
uint32_t systick_safe_read;	 // micros() synchronization
volatile uint32_t scale_cpu_cycles_to_microseconds = 0xFFFFFFFFu / (uint32_t)(500000000u / 1000000u);

// (teensy4) page 411 says "24 MHz XTALOSC can be the external clock source of SYSTICK"
// Testing shows the frequency is actually 100 kHz - but how?  Did NXP really
// hide an undocumented divide-by-240 circuit in the hardware?
#define SYSTICK_EXT_FREQ 100000
void delay(uint32_t msec);
unsigned long micros(void);
void yield(void);

void yield(void)
{

}

void delay(unsigned long msec)
{
	uint32_t start;

	if (msec == 0) return;
	start = micros();
	while (1) {
		while ((micros() - start) >= 1000) {
			if (--msec == 0) return;
			start += 1000;
		}
		yield();
	}
}

void delayMicroseconds(unsigned int us)
{
  if (us == 0)
  {
    return;
  }

  uint32_t start = micros();

  while (us > 0)
  {
    yield();
    while (us > 0 && (micros() - start) >= 1)
    {
      us--;
      start += 1;
    }
  }
}

// Interrupt-compatible version of micros
// Theory: repeatedly take readings of SysTick counter, millis counter and SysTick interrupt pending flag.
// When it appears that millis counter and pending is stable and SysTick hasn't rolled over, use these
// values to calculate micros. If there is a pending SysTick, add one to the millis counter in the calculation.
unsigned long micros( void )
{
  uint32_t ticks, ticks2;
  uint32_t pend, pend2;
  uint32_t count, count2;

  ticks2  = SysTick->VAL;
  pend2   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
  count2  = systick_millis_count;

  do
{
    ticks=ticks2;
    pend=pend2;
    count=count2;
    ticks2  = SysTick->VAL;
    pend2   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
    count2  = systick_millis_count;
  } while ((pend != pend2) || (count != count2) || (ticks < ticks2));

  return ((count+pend) * 1000) + (((SysTick->LOAD  - ticks)*(1048576/(SystemCoreClock/1000000)))>>20) ;
  // this is an optimization to turn a runtime division into two compile-time divisions and
  // a runtime multiplication and shift, saving a few cycles
}

// // Returns the number of microseconds since your program started running.
// // This 32 bit number will roll back to zero after about 71 minutes and
// // 35 seconds.  For a simpler way to build delays or timeouts, consider
// // using elapsedMicros.
// uint32_t micros(void)
// {
// 	uint32_t smc, scc;
// 	do {
// 		__LDREXW(&systick_safe_read);
// 		smc = systick_millis_count;
// 		scc = systick_cycle_count;
// 	} while ( __STREXW(1, &systick_safe_read));
// 	uint32_t cyccnt = DWT->CYCCNT;
// 	asm volatile("" : : : "memory");
// 	uint32_t ccdelta = cyccnt - scc;
// 	uint32_t frac = ((uint64_t)ccdelta * scale_cpu_cycles_to_microseconds) >> 32;
// 	if (frac > 1000) frac = 1000;
// 	uint32_t usec = 1000*smc + frac;
// 	return usec;
// }


void SysTick_Handler(void)
{
	systick_cycle_count = DWT->CYCCNT;
  systick_millis_count++;
}

void init( void )
{
    BOARD_BootClockRUN();
    /* Update the core clock */
    SystemCoreClockUpdate();

    /*allow io mux*/
    CLOCK_EnableClock(kCLOCK_Iomuxc); 

    // /* Set systick reload value to generate 1ms interrupt */
    // if(SysTick_Config(100'000 / 1000U))
    // {
    //     while (1)
    //     {
    //     }
    // }
    #define SYSTICK_EXT_FREQ 100000

    SysTick->LOAD = (SYSTICK_EXT_FREQ / 1000) - 1;
	  SysTick->VAL = 0;
    SysTick->CTRL  = SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;    /* Enable SysTick IRQ and SysTick Timer */	  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // turn on cycle counter;
	  systick_cycle_count = DWT->CYCCNT; // compiled 0, corrected w/1st systick


    // Initialize Analog Controller
    adc_config_t adcConfigStrcut;
     /*
     *  config->enableAsynchronousClockOutput = true;
     *  config->enableOverWrite =               false;
     *  config->enableContinuousConversion =    false;
     *  config->enableHighSpeed =               false;
     *  config->enableLowPower =                false;
     *  config->enableLongSample =              false;
     *  config->referenceVoltageSource =        kADC_ReferenceVoltageSourceVref;
     *  config->samplePeriodMode =              kADC_SamplePeriod2or12Clocks;
     *  config->clockSource =                   kADC_ClockSourceAD;
     *  config->clockDriver =                   kADC_ClockDriver1;
     *  config->resolution =                    kADC_Resolution12Bit;
     */
    ADC_GetDefaultConfig(&adcConfigStrcut);
    ADC_Init(ADC1, &adcConfigStrcut);
#ifdef ADC2
    ADC_Init(ADC2, &adcConfigStrcut);
#endif
#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
    ADC_EnableHardwareTrigger(ADC1, false);
#ifdef ADC2
    ADC_EnableHardwareTrigger(ADC2, false);
#endif
#endif
    
    // waitting for ADC Auto Calibiration
    while (!(kStatus_Success == ADC_DoAutoCalibration(ADC1))){}
#ifdef ADC2
    while (!(kStatus_Success == ADC_DoAutoCalibration(ADC2))){}
#endif

}
