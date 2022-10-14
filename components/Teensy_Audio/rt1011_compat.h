#pragma once

#define F_CPU_ACTUAL 500000000

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
// Teensy 4 core uses interrupt 70 as IRQ_SOFTWARE
#ifndef IRQ_SOFTWARE
  #define IRQ_SOFTWARE Reserved70_IRQn
#endif

#define FLASHMEM __atribute__((section(".text")))

// #define DMA_BITER_ELINKYES_ELINK_MASK            (0x8000U)
#define DMA_TCD_CSR_INTMAJOR DMA_CSR_INTMAJOR(1)
#define DMA_TCD_CSR_INTHALF DMA_CSR_INTHALF(1)
#define DMA_TCD_CSR_DREQ DMA_CSR_DREQ(1)
#define DMA_TCD_CSR_DONE DMA_CSR_DONE(1)
#define DMA_TCD_CSR_ESG DMA_CSR_ESG(1)
#define DMA_TCD_BITER_ELINKYES_LINKCH_MASK DMA_BITER_ELINKYES_LINKCH_MASK
#define DMA_TCD_BITER_ELINKYES_LINKCH DMA_BITER_ELINKYES_LINKCH
#define NVIC_NUM_INTERRUPTS NUMBER_OF_INT_VECTORS
#define IRQ_NUMBER_t IRQn_Type
#define __ARM_ARC_7EM__
#define DMA_TCD_BITER_ELINK ((uint16_t)1<<15) 

#define DMA_TCD_BITER_ELINKYES_ELINK DMA_BITER_ELINKYES_ELINK(1)
#define DMA_SSRT DMA0->SSRT
#define DMA_CINT DMA0->CINT
#define DMA_SERQ DMA0->SERQ
#define DMA_CERQ DMA0->CERQ
#define DMA_CEEI DMA0->CEEI
#define DMA_CERR DMA0->CERR
#define DMA_ERR DMA0->ERR
#define DMA_CDNE DMA0->CDNE
#define DMA_CR DMA0->CR
#define CCM_CCGR5 CCM->CCGR5
#define CCM_CCGR5_DMA CCM_CCGR5_CG3
#define CCM_CCGR_ON 1
#define IRQ_DMA_CH0 DMA0_IRQn
#define DMA_TCD_CSR_MAJORLINKCH_MASK DMA_CSR_MAJORLINKCH_MASK
#define DMA_TCD_CSR_MAJORLINKCH DMA_CSR_MAJORLINKCH
#define DMA_TCD_CSR_MAJORELINK DMA_CSR_MAJORELINK(1)

#define DMAMUX_CHCFG0 DMAMUX->CHCFG[0]

#define _VectorsRam __Vectors
// What is _VectorsRam and where is it located?
// How can we attachInterrupt to a dynamically allocated DMA channel?
#ifdef __cplusplus
extern "C" void (* _VectorsRam[NVIC_NUM_INTERRUPTS+16])(void);
static inline void attachInterruptVector(IRQn_Type irq, void (*function)(void)) __attribute__((always_inline, unused));
static inline void attachInterruptVector(IRQn_Type irq, void (*function)(void)) { _VectorsRam[irq + 16] = function; asm volatile("": : :"memory"); }
#else
extern void (* _VectorsRam[NVIC_NUM_INTERRUPTS+16])(void);
static inline void attachInterruptVector(enum IRQn_Type irq, void (*function)(void)) __attribute__((always_inline, unused));
static inline void attachInterruptVector(enum IRQn_Type irq, void (*function)(void)) { _VectorsRam[irq + 16] = function; asm volatile("": : :"memory"); }
#endif
