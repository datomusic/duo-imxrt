#ifndef CORE_EXTRAS_H_FLCU2I6S
#define CORE_EXTRAS_H_FLCU2I6S


struct digital_pin_bitband_and_config_table_struct {
  volatile uint32_t *reg;
  volatile uint32_t *config;
  uint8_t mask;
};

digital_pin_bitband_and_config_table_struct digital_pin_to_info_PGM[0];

#define digitalPinToBitMask(pin) (digital_pin_to_info_PGM[(pin)].mask)
#define portSetRegister(pin) ((digital_pin_to_info_PGM[(pin)].reg + 4))
#define portClearRegister(pin) ((digital_pin_to_info_PGM[(pin)].reg + 8))

#define ARM_DEMCR CoreDebug->DEMCR
#define ARM_DEMCR_TRCENA CoreDebug_DEMCR_TRCENA_Msk
#define ARM_DWT_CTRL DWT->CTRL
#define ARM_DWT_CTRL_CYCCNTENA DWT_CTRL_CYCCNTENA_Msk
#define ARM_DWT_CYCCNT DWT->CYCCNT

#define interrupts __enable_irq
#define noInterrupts __disable_irq

#define F_CPU_ACTUAL DEFAULT_SYSTEM_CLOCK

#endif /* end of include guard: CORE_EXTRAS_H_FLCU2I6S */
