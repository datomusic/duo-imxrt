#ifndef USB_H_EGTUA9NZ
#define USB_H_EGTUA9NZ

#include <stdint.h>

namespace DatoUSB {

void init();
bool background_update();
bool midi_read(uint8_t packet[4]);
void midi_send(const uint8_t packet[4]);

}; // namespace DatoUSB

#endif /* end of include guard: USB_H_EGTUA9NZ */
