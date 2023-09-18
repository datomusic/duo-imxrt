#include "leds.h"
#include "Arduino.h"
#include "channel.h"
#include "flexio_led_driver.h"
#include "fsl_dmamux.h"

static const unsigned PIXEL_COUNT = 19;
static const uint32_t BYTE_COUNT = 3 * PIXEL_COUNT + 3;
static uint32_t prepped_pixels[BYTE_COUNT];

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

namespace LEDs {

void init(void) {
  setup_flexio_leds();
  channel.reset();
}

void setBrightness(int brightness) { _brightness = brightness; }
void show(const Pixel *const pixels, int pixel_count) {
  if (pixel_count > PIXEL_COUNT) {
    pixel_count = PIXEL_COUNT;
  }

  uint8_t *bytes = (uint8_t *)pixels;
  for (uint32_t ind = 0; ind < (pixel_count * 3); ++ind) {
    prepped_pixels[ind] = spread4(bytes[ind]) << 3;
  }

  // show_bytes((uint8_t *)pixels, pixel_count * 3);
  begin_show();
  prepare_write(prepped_pixels, BYTE_COUNT);
  channel.enable();
  // end_show();
}
} // namespace LEDs
