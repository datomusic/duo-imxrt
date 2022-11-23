#ifndef USB_H_EGTUA9NZ
#define USB_H_EGTUA9NZ

#include "tusb.h"
#include <stdint.h>

namespace DatoUSB {

void init();
inline void background_update(void) {
#if CFG_TUSB_OS == OPT_OS_NONE
  tud_task();
#endif
}

bool midi_read(uint8_t packet[4]);
void midi_send(int cable_num, const uint8_t packet[], const int packet_len);

}; // namespace DatoUSB

#endif /* end of include guard: USB_H_EGTUA9NZ */
