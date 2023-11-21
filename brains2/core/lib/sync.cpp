#include "sync.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "board.h"
#include "pins.h"
#include <Arduino.h>

#define SYNC_NVIC_PRIORITY 150
#define SYNC_PIN_IRQ GPIO1_Combined_0_15_IRQn
#define SYNC_PIN_IRQ_HANDLER GPIO1_Combined_0_15_IRQHandler


namespace Sync {
    uint32_t syncPinState;

    void handleSyncInterrupt(void) {
      GPIO_PortClearInterruptFlags(SYNC_IN_PORT, 1U << SYNC_IN_PIN);
      Sync::syncPinState = 1;
    }

    void init() {
        IOMUXC_SetPinMux(SYNC_IN_PINMUX, 0U);
        gpio_pin_config_t sync_in_config = { kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge};
        GPIO_SetPinInterruptConfig(SYNC_IN_PORT, SYNC_IN_PIN, kGPIO_IntFallingEdge);
        NVIC_SetPriority(SYNC_PIN_IRQ, SYNC_NVIC_PRIORITY);
        NVIC_SetVector(SYNC_PIN_IRQ, (uint32_t)&Sync::handleSyncInterrupt);
        EnableIRQ(SYNC_PIN_IRQ);
        GPIO_PinInit(SYNC_IN_PORT, SYNC_IN_PIN, &sync_in_config);
        GPIO_PortClearInterruptFlags(SYNC_IN_PORT, 1 << SYNC_IN_PIN);
        GPIO_PortEnableInterrupts(SYNC_IN_PORT, 1 << SYNC_IN_PIN);

        IOMUXC_SetPinMux(SYNC_OUT_PINMUX, 0U);
        gpio_pin_config_t sync_out_config = { kGPIO_DigitalOutput, 0};
        GPIO_PinInit(SYNC_OUT_PORT, SYNC_OUT_PIN, &sync_out_config);
    }

    uint32_t read() {
       if(syncPinState == 1) {
         syncPinState = 0;
         return HIGH;
      }
      return LOW;
    }
    
    void write(uint8_t value) {
        GPIO_PinWrite(SYNC_OUT_PORT, SYNC_OUT_PIN, value);
    }

    bool detect(){
      return pinRead(SYNC_DETECT_PIN);
    }
}
