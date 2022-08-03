// Used by FastLED for various timings.
#define F_CPU DEFAULT_SYSTEM_CLOCK

// Disable FastLED reliance on digitalPinToBitMask and friends.
// These are only used in the FastLed Pin class, which is legacy code anyway,
// replaced by FastPin.
// Source: https://github.com/FastLED/FastLED/issues/766
#define FASTLED_NO_PINMAP

#define ARM_DWT_CYCCNT DWT->CYCCNT
#define FASTLED_TEENSY4
#define HAS_HARDWARE_PIN_SUPPORT
#define HAS_CLOCKLESS
void delayNanoseconds(int x){}
/* #define ARM_HARDWARE_SPI */

// Copied from components/FastLED/platforms/arm/stm32/led_sysdefs_arm_stm32.h
#define cli()  __disable_irq(); __disable_fault_irq();
#define sei() __enable_irq(); __enable_fault_irq();

