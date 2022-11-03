#define __IMXRT1062__ true

#define ARM_DWT_CYCCNT 1
#define DMAMEM
#define FLASHMEM


#ifndef NVIC_ENABLE_IRQ
  #define NVIC_ENABLE_IRQ NVIC_EnableIRQ
#endif
#ifndef NVIC_DISABLE_IRQ
  #define NVIC_DISABLE_IRQ NVIC_DisableIRQ
#endif
#ifndef NVIC_SET_PRIORITY
  #define NVIC_SET_PRIORITY NVIC_SetPriority
#endif
#ifndef ARM_DWT_CYCCNT
  #define ARM_DWT_CYCCNT DWT->CYCCNT
#endif