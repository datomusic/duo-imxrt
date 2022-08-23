#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "leds.h"
#include "pin_mux.h"

using LEDs::Pixel;

/*
int main(void) {
  //Board pin init
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  LEDs::init();
  init(); // Seeeduino init

  int counter = 0;

  const int PIXEL_COUNT = 4;
  Pixel pixels[PIXEL_COUNT] = {Pixel{0, 0xFF, 0}, Pixel{0, 0, 0},
                               Pixel{0, 0xFF, 0}, Pixel{0xFF, 0x0, 0x0}};

  while (1) {
    if (++counter > 1000) {
      ++pixels[1].b;
      counter = 0;
      LEDs::show(pixels, PIXEL_COUNT);
    }

    delayMicroseconds(1);
  }
} */

int main(void) {
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();
  LEDs::init();
  init(); // Seeeduino init

#define BUFFER_LEN (64)
  spi_master_handle_t spiHandle;
  spi_master_config_t masterConfig;
  spi_transfer_t xfer;
  volatile bool isFinished = false;
  const uint8_t sendData[BUFFER_LEN] = [......];
  uint8_t receiveBuff[BUFFER_LEN];
  void SPI_UserCallback(SPI_Type * base, spi_master_handle_t * handle,
                        status_t status, void *userData) {
    isFinished = true;
  }
  void main(void) {
    //...
    SPI_MasterGetDefaultConfig(&masterConfig);
    SPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
    SPI_MasterTransferCreateHandle(SPI0, &spiHandle, SPI_UserCallback, NULL);
    // Prepare to send.
    xfer.txData = sendData;
    xfer.rxData = receiveBuff;
    xfer.dataSize = sizeof(sendData);
    // Send out.
    SPI_MasterTransferNonBlocking(SPI0, &spiHandle, &xfer);
    // Wait send finished.
    while (!isFinished) {
    }
    // ...
  }
}
