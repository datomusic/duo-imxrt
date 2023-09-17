#include "Arduino.h"
#include "flexio_led_driver.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "lib/board_init.h"

#define EDMA_CHANNEL (1U)

const uint8_t led_pins[3] = {GPIO_08, GPIO_07, GPIO_06};

#define write_led(pin, value)

void led_show(const unsigned pin, bool on) {
  if (on) {
    analogWrite(led_pins[pin], 255);
  } else {
    analogWrite(led_pins[pin], 0);
  }
  // GPIO_PinWrite(LED2_PORT, LED2_PIN, on);
}

void flash_led(const uint32_t ms) {
  led_show(0, true);
  delayMicroseconds(1000 * ms);
  led_show(0, false);
}

void flash_led() { flash_led(100); }

void init_debug_led() {
  // IOMUXC_SetPinMux(LED2_PINMUX, 0U);
  // gpio_pin_config_t led2_config = {kGPIO_DigitalOutput, 0};
  // GPIO_PinInit(LED2_PORT, LED2_PIN, &led2_config);

  // pinMode(PIN_SYN_ADDR0, OUTPUT);
  // pinMode(PIN_SYN_ADDR1, OUTPUT);
  // pinMode(PIN_SYN_ADDR2, OUTPUT);

  flash_led(100);
  delayMicroseconds(1000 * 500);
  flash_led(1000);
  delayMicroseconds(1000 * 2000);
}

const unsigned PIXEL_COUNT = 19;

const uint32_t BYTE_COUNT = 3 * PIXEL_COUNT;
uint8_t bytes[BYTE_COUNT];

edma_handle_t g_EDMA_Handle;
edma_config_t userConfig;
edma_transfer_config_t transferConfig;

uint32_t prepared[BYTE_COUNT];

volatile bool g_Transfer_Done = false;

void loop() {
  uint32_t destination = FLEXIO1->SHIFTBUFBIS[data_shifter_id];

  uint8_t counter = 0;
  for (;;) {
    // if (true) {
    for (unsigned i = 0; i < BYTE_COUNT; ++i) {
      bytes[i] = 0;
    }

    const uint8_t p = (counter % PIXEL_COUNT) * 3;
    bytes[p] = 255;
    bytes[p + 1] = 255;
    bytes[p + 2] = 255;

    for (uint32_t ind = 0; ind < BYTE_COUNT; ++ind) {
      prepared[ind] = spread4(bytes[ind]) << 3;
    }

    // show_prepared(prepared, BYTE_COUNT);
    // show_bytes(bytes, BYTE_COUNT);

    led_show(1, true);
    // delayMicroseconds(500 * 1000);
    g_Transfer_Done = false;
    EDMA_PrepareTransfer(&transferConfig, prepared, sizeof(prepared[0]),
                         (void *)destination, sizeof(destination),
                         sizeof(prepared[0]), BYTE_COUNT,
                         kEDMA_MemoryToPeripheral);

    const status_t res = EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    if (res == kStatus_Success) {
      led_show(2, true);
    }

    // Enable DMA input on shifter
    uint32_t dma_reg = 1 << data_shifter_id;
    FLEXIO1->SHIFTSDEN = dma_reg;

    EDMA_StartTransfer(&g_EDMA_Handle);
    /* Wait for EDMA transfer finish */
    while (g_Transfer_Done != true) {
    }

    // delayMicroseconds(1000 * 1000);
    led_show(1, false);

    ++counter;

    // delayMicroseconds(100000);
    led_show(2, false);
  }
}

void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone,
                   uint32_t tcds) {
  if (transferDone) {
    g_Transfer_Done = true;
  }
}

int main(void) {
  board_init();
  init_debug_led();
  setup_flexio_leds();

  const uint32_t channel = 0;

  DMAMUX_Init(DMAMUX);
  DMAMUX_EnableAlwaysOn(DMAMUX, channel, true);
  DMAMUX_EnableChannel(DMAMUX, channel);

  EDMA_GetDefaultConfig(&userConfig);
  EDMA_Init(DMA0, &userConfig);
  EDMA_CreateHandle(&g_EDMA_Handle, DMA0, channel);

  EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);

  loop();
  return 0;
}
