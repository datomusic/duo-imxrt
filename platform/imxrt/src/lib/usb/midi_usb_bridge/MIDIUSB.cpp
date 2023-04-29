#include "MIDIUSB.h"
#include "lib/usb/usb.h"

MidiUSB_ MidiUSB;

midiEventPacket_t MidiUSB_::read(void) {
  midiEventPacket_t result{0, 0, 0, 0};
  uint8_t recv_packet[4];

  if (DatoUSB::midi_read(recv_packet)) {
    result.header = recv_packet[0];
    result.byte1 = recv_packet[1];
    result.byte2 = recv_packet[2];
    result.byte3 = recv_packet[3];
  }

  return result;
}

void MidiUSB_::flush(void) {}

void MidiUSB_::sendMIDI(const midiEventPacket_t event) {
  const uint8_t packet[4] = {event.header, event.byte1, event.byte2,
                             event.byte3};
  DatoUSB::midi_send(packet);
}
