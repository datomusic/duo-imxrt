#include "midi.h"

MIDIActuator::MIDIActuator()
    : usbTransport(0), usb(usbTransport), serialTransport(Serial),
      serial(serialTransport) {
}
