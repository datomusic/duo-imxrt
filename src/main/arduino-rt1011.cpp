#include "pin_mux.h"
#include "Arduino.h"

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


    while (1)
    {

    }

}

