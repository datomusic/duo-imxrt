#include "sync.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "board.h"

namespace Sync {
    void init() {
        IOMUXC_SetPinMux(SYNC_IN_PINMUX, 0U);
        gpio_pin_config_t sync_in_config = { kGPIO_DigitalInput, 0, kGPIO_IntRisingEdge};
        GPIO_PinInit(SYNC_IN_PORT, SYNC_IN_PIN, &sync_in_config);

        IOMUXC_SetPinMux(SYNC_OUT_PINMUX, 0U);
        gpio_pin_config_t sync_out_config = { kGPIO_DigitalOutput, 0};
        GPIO_PinInit(SYNC_OUT_PORT, SYNC_OUT_PIN, &sync_out_config);
        
        IOMUXC_SetPinMux(LED2_PINMUX, 0U);
        gpio_pin_config_t led2_config = { kGPIO_DigitalOutput, 0};
        GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);
    }
    

    uint32_t read() {
        return GPIO_PinRead(SYNC_IN_PORT, SYNC_IN_PIN);
    }
    
    void write(uint8_t value) {
        GPIO_PinWrite(SYNC_OUT_PORT, SYNC_OUT_PIN, value);
    }
}