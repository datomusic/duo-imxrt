#include "leds.h"
#include "flexio_led_driver.h"

namespace LEDs {

void init(void) { setup_flexio_leds(); }

void setBrightness(int brightness) { _brightness = brightness; }
void show(const Pixel *const pixels, const int pixel_count) {
  show_bytes((uint8_t *)pixels, pixel_count * 3);
}
} // namespace LEDs
