#include "pin_mux.h"
#include "Arduino.h"
#include "Audio.h"

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=174,384
AudioOutputMQS           mqs1;           //xy=356,386
AudioConnection          patchCord1(sine1, 0, mqs1, 0);
AudioConnection          patchCord2(sine1, 0, mqs1, 1);
// GUItool: end automatically generated code

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

