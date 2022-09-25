#ifndef USB_H_EGTUA9NZ
#define USB_H_EGTUA9NZ

namespace DatoUSB{
  void init();
  bool background_update();
  void midi_send(int cable_num, const uint8_t packet[], const int packet_len);
};


#endif /* end of include guard: USB_H_EGTUA9NZ */
