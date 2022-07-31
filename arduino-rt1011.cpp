#include "pin_mux.h"
#include "Arduino.h"
#define TEENSYDUINO 999
#include "Audio.h"
// #include <FastLED.h>
#include "bs814a.h"

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=305,275
AudioOutputMQS           mqs1;           //xy=479,272
AudioConnection          patchCord1(sine1, 0, mqs1, 0);
AudioConnection          patchCord2(sine1, 0, mqs1, 1);
// GUItool: end automatically generated code

// CRGB leds[19];

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

    // FastLED.addLeds<WS2812, 23, GRB>(leds, 19);
    AudioMemory(100);
    sine1.amplitude(0.5F);
    sine1.frequency(440.0F);

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