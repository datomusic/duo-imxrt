#define UART_RETRY_TIMES true
#include "pin_mux.h"
#include "Arduino.h"
// #include "Audio.h"
#include "bs814a.h"
#include <MIDI.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI); 
#define LED2_PINMUX IOMUXC_GPIO_07_GPIOMUX_IO07
#define LED2_PORT GPIO1
#define LED2_PIN 7U

// GUItool: begin automatically generated code
// AudioSynthWaveformSine   sine1;          //xy=305,275
// AudioOutputMQS           mqs1;           //xy=479,272
// AudioConnection          patchCord1(sine1, 0, mqs1, 0);
// AudioConnection          patchCord2(sine1, 0, mqs1, 1);
// GUItool: end automatically generated code

// CRGB leds[19];

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
    /* Board pin init */
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    //BOARD_InitNeopixels();
    init(); // Seeeduino init
    IOMUXC_SetPinMux(LED2_PINMUX, 0U);
    gpio_pin_config_t led2_config = { kGPIO_DigitalOutput, 0};
    GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);
    //uint8_t blue[] = {0x00,0x00,0xff};
    // board_rgb_write(blue);
    GPIO_PinWrite(LED2_PORT, LED2_PIN, 1);
    delay(1000);
    GPIO_PinWrite(LED2_PORT, LED2_PIN, 0);
    Serial.begin(31250U); //This is needed to configure the UART peripheral correctly
    MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    
    #define PIN_MQS_R D7
    #define PIN_AMP_MUTE PIN_A2
    #define PIN_HP_ENABLE 23U
    pinMode(PIN_MQS_R, OUTPUT); // AD_02, MQS pin, pin 58
    pinMode(PIN_AMP_MUTE, OUTPUT); // AD_10, /AMP_MUTE pin 47
    pinMode(PIN_HP_ENABLE, OUTPUT); // AD_11, /HP_ENABLE pin 46
    digitalWrite(PIN_AMP_MUTE, LOW);
    digitalWrite(PIN_HP_ENABLE, HIGH);
    
    // AudioMemory(100);
    // sine1.amplitude(0.5F);
    // sine1.frequency(440.0F);


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
