#include "Arduino.h"
#include "channel.h"
#include "flexio_led_driver.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
#include "lib/board_init.h"

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

void init_debug_led() { flash_led(100); }

Channel channel;
void prepare_write(uint32_t *buffer, const uint16_t length) {
  channel.disable();
  channel.set_disable_on_completion(true);
  DMAMUX_EnableChannel(DMAMUX, DMA_SIGNAL_ID);
  channel.set_source_linear_buffer(buffer, length);

  volatile uint32_t *destination_address =
      &(FLEXIO1->SHIFTBUFBIS[data_shifter_id]);
  channel.set_destination_hardware((uint32_t *)destination_address);

  channel.set_minor_loop_bytes(sizeof(uint32_t));

  channel.set_transfer_iterations(length);

  // Enable DMA input on shifter
  uint32_t dma_reg = 1 << data_shifter_id;
  FLEXIO1->SHIFTSDEN = dma_reg;
}

void init_dma() {
  edma_config_t userConfig;
  EDMA_GetDefaultConfig(&userConfig);
  EDMA_Init(DMA0, &userConfig);

  DMAMUX_Init(DMAMUX);
  DMAMUX_SetSource(DMAMUX, DMA_SIGNAL_ID, 0);
}

const unsigned PIXEL_COUNT = 19;

const uint32_t BYTE_COUNT = 3 * PIXEL_COUNT + 3;
uint8_t bytes[BYTE_COUNT];

uint32_t prepped_pixels[BYTE_COUNT];

uint8_t counter = 0;
void render_frame() {
  for (unsigned i = 0; i < BYTE_COUNT; ++i) {
    bytes[i] = 0;
  }

  const uint8_t p = (counter % PIXEL_COUNT) * 3;
  bytes[p] = 255;
  bytes[p + 1] = 255;
  bytes[p + 2] = 255;

  ++counter;
}

int main(void) {
  board_init();
  init_debug_led();
  setup_flexio_leds();
  init_dma();
  channel.reset();

  for (;;) {
    render_frame();

    for (uint32_t ind = 0; ind < BYTE_COUNT; ++ind) {
      prepped_pixels[ind] = spread4(bytes[ind]) << 3;
    }

    led_show(1, true);
    // begin_show();

    led_show(2, true);
    prepare_write(prepped_pixels, BYTE_COUNT);
    channel.enable();

    // end_show();
    led_show(2, false);

    delayMicroseconds(10000);
  }
  return 0;
}
