#include "pin_mux.h"
#include "Arduino.h"
#include "Audio.h"

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

    pinMode(PIN_LED_1, OUTPUT);
    while (1)
    {
        digitalWrite(PIN_LED_1, HIGH);
        delay(500);
        digitalWrite(PIN_LED_1, LOW);
        delay(500);
    }

}

