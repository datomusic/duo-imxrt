#include "Arduino.h"
#include "lib/board_init.h"
#include "lib/leds.h"
#include "lib/pin_mux.h"
#include "lib/usb/usb.h"
#include <USB-MIDI.h>

USBMIDI_CREATE_DEFAULT_INSTANCE();

#define LED2_PINMUX IOMUXC_GPIO_07_GPIOMUX_IO07
#define LED2_PORT GPIO1
#define LED2_PIN 7U

void handleNoteOn(byte channel, byte note, byte velocity) {
  GPIO_PinWrite(LED2_PORT, LED2_PIN, 1);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  GPIO_PinWrite(LED2_PORT, LED2_PIN, 0);
}

int main(void) {
  board_init();
  DatoUSB::init();

  IOMUXC_SetPinMux(LED2_PINMUX, 0U);
  gpio_pin_config_t led2_config = {kGPIO_DigitalOutput, 0};
  GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);

  GPIO_PinWrite(LED2_PORT, LED2_PIN, 1);
  delay(1000);
  GPIO_PinWrite(LED2_PORT, LED2_PIN, 0);

  MIDI.begin(MIDI_CHANNEL_OMNI); // Listen to all incoming messages
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  while (true) {
    DatoUSB::background_update();
    MIDI.read();
    delayMicroseconds(1000);
  }
}
