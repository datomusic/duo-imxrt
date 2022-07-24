#ifndef __INC_LED_SYSDEFS_ARM_MXRT1011_H
#define __INC_LED_SYSDEFS_ARM_MXRT1011_H

#define FASTLED_ARM

#ifndef INTERRUPT_THRESHOLD
#define INTERRUPT_THRESHOLD 1
#endif

// Default to allowing interrupts
#ifndef FASTLED_ALLOW_INTERRUPTS
#define FASTLED_ALLOW_INTERRUPTS 1
#endif

#if FASTLED_ALLOW_INTERRUPTS == 1
#define FASTLED_ACCURATE_CLOCK
#endif


// Define the register types
typedef volatile       uint32_t RoReg; /**< Read only 8-bit register (volatile const unsigned int) */
typedef volatile       uint32_t RwReg; /**< Read-Write 8-bit register (volatile unsigned int) */

// extern volatile uint32_t systick_millis_count;
// #  define MS_COUNTER systick_millis_count

// // Teensy4 provides progmem
// #ifndef FASTLED_USE_PROGMEM
// #define FASTLED_USE_PROGMEM 1
// #endif

#endif
