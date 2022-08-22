#ifndef CORE_EXTRAS_H_FLCU2I6S
#define CORE_EXTRAS_H_FLCU2I6S

void interrupts() { __enable_irq(); }
void noInterrupts() { __disable_irq(); }

/* #define portSetRegister(pin)    ((volatile uint8_t
 * *)(digital_pin_to_info_PGM[(pin)].reg + 32)) */
/* #define portClearRegister(pin)  ((volatile uint8_t
 * *)(digital_pin_to_info_PGM[(pin)].reg + 64)) */

struct PortDevice{
    uint8_t reg_tmp;
};

struct PinInfo{
    PortDevice* gpio_device;
    uint8_t gpio_bit;
    uint8_t regs_odr;
};


struct digital_pin_bitband_and_config_table_struct {
        volatile uint32_t *reg;
        volatile uint32_t *config;
	uint8_t mask;
};

digital_pin_bitband_and_config_table_struct digital_pin_to_info_PGM[0];
// compatibility macros
#define digitalPinToPort(pin) (pin)
#define digitalPinToBitMask(pin) (digital_pin_to_info_PGM[(pin)].mask)
#define portOutputRegister(pin) ((digital_pin_to_info_PGM[(pin)].reg + 0))
#define portSetRegister(pin)    ((digital_pin_to_info_PGM[(pin)].reg + 4))
#define portClearRegister(pin)  ((digital_pin_to_info_PGM[(pin)].reg + 8))
#define portToggleRegister(pin) ((digital_pin_to_info_PGM[(pin)].reg + 12))
#define portInputRegister(pin)  ((digital_pin_to_info_PGM[(pin)].reg + 16))
#define portModeRegister(pin)   ((digital_pin_to_info_PGM[(pin)].reg + 20))
#define portConfigRegister(pin) ((digital_pin_to_info_PGM[(pin)].config))
#define digitalPinToPortReg(pin) (portOutputRegister(pin))


#define ARM_DEMCR
#define ARM_DEMCR_TRCENA;
#define ARM_DWT_CTRL
#define ARM_DWT_CTRL_CYCCNTENA;



#endif /* end of include guard: CORE_EXTRAS_H_FLCU2I6S */
