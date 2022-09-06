#include "pin_mux.h"
#include "Arduino.h"
#define TEENSYDUINO 999
// #include "Audio.h"
#include "sync.h"
#include "board.h"

// GUItool: begin automatically generated code
// AudioSynthWaveformSine   sine1;          //xy=305,275
// AudioOutputMQS           mqs1;           //xy=479,272
// AudioConnection          patchCord1(sine1, 0, mqs1, 0);
// AudioConnection          patchCord2(sine1, 0, mqs1, 1);
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

