#include "pin_mux.h"
#include "Arduino.h"
#define TEENSYDUINO 999

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
    //BOARD_InitNeopixels();
    init(); // Seeeduino init

    while (1)
    {
       
    }
}
