#ifdef TEENSYDUINO
  #undef TEENSYDUINO
#endif
#define TEENSYDUINO 999

// Compatibility defines between Teensy core and NXP SDK
#ifndef NVIC_SET_PENDING
  #define NVIC_SET_PENDING NVIC_SetPendingIRQ
#endif
#ifndef NVIC_ENABLE_IRQ
  #define NVIC_ENABLE_IRQ NVIC_EnableIRQ
#endif
#ifndef NVIC_SET_PRIORITY
  #define NVIC_SET_PRIORITY NVIC_SetPriority
#endif
#ifndef ARM_DWT_CYCCNT
  #define ARM_DWT_CYCCNT DWT->CYCCNT
#endif
#ifndef NVIC_DISABLE_IRQ
  #define NVIC_DISABLE_IRQ NVIC_DisableIRQ
#endif
// It seems the RT1011 interrupts are not the same as the RT1050/RT1060.
// Teensy 4 core uses interrupt 80 as IRQ_SOFTWARE
#ifndef IRQ_SOFTWARE
  #define IRQ_SOFTWARE Reserved70_IRQn
#endif

#define FLASHMEM __atribute__((section(".text")))