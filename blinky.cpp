
#include "pin_mux.h"
// #include "delay.h"
#include "Arduino.h"
#include "AudioStream.h"
#include "DMAChannel.h"
// #include "output_mqs.h"
// #include <FastLED.h>

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

    // uint8_t teal[] = {0x00,0xf0,0xc0};
    // uint8_t yellow[] = {0xff,0xff,0x00};
    uint8_t blue[] = {0x00,0x00,0xff};
    // uint8_t black[] = {0x00,0x00,0x00};
    board_rgb_write(blue);
    SerialUSB.begin(9600);
    // bool g_pinSet = false;
    // unsigned long timenow;
    pinMode(USER_LED, OUTPUT);

    while (1)
    {
        /* Delay 1000 ms */
        digitalWrite(USER_LED, LOW);
        delay(10);
        uint32_t pot_value = analogRead(D11);
        uint32_t pot_value2 = analogRead(D12);
        uint8_t b = (uint8_t) (255-(pot_value >> 4));
        uint8_t r = (uint8_t) (255-(pot_value2 >> 4));
        uint8_t potRGB[] = {r,b,b};
        board_rgb_write(potRGB);
        digitalWrite(USER_LED,HIGH);
        SerialUSB.print(pot_value);
        delay(10);
    }
}
