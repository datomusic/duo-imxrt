#include "pin_mux.h"
#include "Arduino.h"
// #include "AudioStream.h"
// #include "DMAChannel.h"
// #include "output_mqs.h"
// #include <FastLED.h>
#include "bs814a.h"

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
    BOARD_InitNeopixels();
    init(); // Seeeduino init

    uint8_t blue[] = {0x00,0x00,0xff};
    board_rgb_write(blue);
    BS814A_begin();

    while (1)
    {
        uint8_t value = BS814A_readRaw();
        uint8_t g = (1-bitRead(value, 1)) * 100;
        uint8_t b = (1-bitRead(value, 2)) * 100;
        uint8_t r = (1-bitRead(value, 3)) * 100;
        uint8_t potRGB[] = {r,g,b};
        board_rgb_write(potRGB);
        delay(10);
    }
}