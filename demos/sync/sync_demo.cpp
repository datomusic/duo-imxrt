#include "pin_mux.h"
#include "Arduino.h"
#define TEENSYDUINO 999
#include "sync.h"
#include "board.h"

/*!
 * @brief Main function
 */
int main(void)
{
    /* Board pin init */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    init(); // Seeeduino init
    Sync::init(); 
    
    GPIO_PinWrite(SYNC_OUT_PORT, SYNC_OUT_PIN, 1);
    delay(1000);
    GPIO_PinWrite(SYNC_OUT_PORT, SYNC_OUT_PIN, 0);
    delay(1000);
    while (1)
    {
        GPIO_PinWrite(SYNC_OUT_PORT, SYNC_OUT_PIN, GPIO_PinRead(SYNC_IN_PORT, SYNC_IN_PIN));
        GPIO_PinWrite(LED2_PORT, LED2_PIN, GPIO_PinRead(SYNC_DETECT_PORT, SYNC_DETECT_PIN));
        delay(10);
    }

}

