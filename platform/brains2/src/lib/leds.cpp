#include "leds.h"
#include "Arduino.h"
#include "channel.h"
#include "flexio_led_driver.h"
#include "fsl_dmamux.h"

static const unsigned PIXEL_COUNT = 19;
static const uint32_t BYTE_COUNT = 3 * PIXEL_COUNT + 3;
uint8_t bytes[BYTE_COUNT];
uint32_t prepped_pixels[BYTE_COUNT];

static Channel channel;
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

namespace LEDs {

void init(void) {
  setup_flexio_leds();
}

void setBrightness(int brightness) { _brightness = brightness; }
void show(const Pixel *const pixels, int pixel_count) {
  render_frame();
  for (uint32_t ind = 0; ind < BYTE_COUNT; ++ind) {
    prepped_pixels[ind] = spread4(bytes[ind]) << 3;
  }

  prepare_write(prepped_pixels, BYTE_COUNT);
  channel.enable();
}
} // namespace LEDs
