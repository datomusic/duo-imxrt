#include "lib/pin_mux.h"
#include "Arduino.h"
#include "lib/board_init.h"
#define TEENSYDUINO 999
#include "lib/sync.h"
#include "lib/board.h"

/*!
 * @brief Main function
 */
int main(void)
{
    board_init();
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

