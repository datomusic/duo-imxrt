#define UART_RETRY_TIMES true
#include "lib/pin_mux.h"
#include "Arduino.h"
#include "lib/board_init.h"
// #include "Audio.h"
#include <MIDI.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI); 
#define LED2_PINMUX IOMUXC_GPIO_07_GPIOMUX_IO07
#define LED2_PORT GPIO1
#define LED2_PIN 7U

void handleNoteOn(byte channel, byte note, byte velocity) {
    GPIO_PinWrite(LED2_PORT, LED2_PIN, 0);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
    GPIO_PinWrite(LED2_PORT, LED2_PIN, 1);
}

/*!
 * @brief Main function
 */
int main(void)
{
    board_init();

    IOMUXC_SetPinMux(LED2_PINMUX, 0U);
    gpio_pin_config_t led2_config = { kGPIO_DigitalOutput, 0};
    GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);

    GPIO_PinWrite(LED2_PORT, LED2_PIN, 1);
    delay(1000);
    GPIO_PinWrite(LED2_PORT, LED2_PIN, 0);

    Serial.begin(31250U); //This is needed to configure the UART peripheral correctly
    MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    
    while (1)
    {
        MIDI.read();
        MIDI.sendNoteOn(70, 127, 1);
        delay(200);
        MIDI.sendNoteOff(70, 127, 1);
        delay(1000);
        MIDI.sendNoteOn(58, 127, 1);
        delay(200);
        MIDI.sendNoteOff(58, 127, 1);
        delay(1000);
    }
}
