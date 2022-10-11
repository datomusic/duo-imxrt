#include "pin_mux.h"
#include "Arduino.h"
#include "Audio.h"
#include "clock_config.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"

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

    CLOCK_EnableClock(kCLOCK_Mqs);  // TODO: this should go into output_mqs.cpp but doesn't work there
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_02_MQS_RIGHT, 0);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_01_MQS_LEFT, 0);

    AudioMemory(64);
    AudioNoInterrupts();
    sine1.frequency(120);
    sine1.amplitude(0.5);
    AudioInterrupts();

    pinMode(PIN_LED_1, OUTPUT);
    pinMode(PIN_AMP_MUTE, OUTPUT);
    pinMode(PIN_HP_ENABLE, OUTPUT);
    
    digitalWrite(PIN_AMP_MUTE, LOW);
    digitalWrite(PIN_HP_ENABLE, HIGH);
    
    while (1)
    {   
        digitalWrite(PIN_LED_1, HIGH);
        delay(500);
        digitalWrite(PIN_LED_1, LOW);
        delay(500);
    }

}
