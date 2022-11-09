#include "lib/usb/usb.h"
#include "device/usbd.h"
#include "fsl_clock.h"
#include "tusb.h"

#ifndef USB_DEVICE_INSTANCE
#define USB_DEVICE_INSTANCE 0
#endif

bool DatoUSB::background_update(void) {
  if (tusb_inited()) {
#if CFG_TUSB_OS == OPT_OS_NONE
    tud_task();
#endif
    return true;
  } else {
    return false;
  }
}

bool DatoUSB::midi_read(uint8_t packet[4]) {
  bool ret = false;
  if (tud_midi_available()) {
    tud_midi_packet_read(packet);
    ret = true;
  } 

  return ret;
}

void DatoUSB::midi_send(const int cable_num, const uint8_t packet[],
                        const int packet_len) {
  tud_midi_stream_write(cable_num, packet, packet_len);
}

static void usb_irq_handler(int instance) {
  if (instance == USB_DEVICE_INSTANCE) {
    tud_int_handler(instance);
  }
}

// These are definitions for machines where sizeof(int) == sizeof(void*),
// regardless for actual size.
typedef int mp_int_t; // must be pointer size

static void init_usb_instance(mp_int_t instance) {
  if (instance < 0) {
    return;
  }
  USBPHY_Type *usb_phy;
  (void)instance;
  usb_phy = USBPHY;
  CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, 480000000U);
  CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, 480000000U);

  // Enable PHY support for Low speed device + LS via FS Hub
  usb_phy->CTRL |=
      USBPHY_CTRL_SET_ENUTMILEVEL2_MASK | USBPHY_CTRL_SET_ENUTMILEVEL3_MASK;

  // Enable all power for normal operation
  usb_phy->PWD = 0;

  // TX Timing
  uint32_t phytx = usb_phy->TX;
  phytx &= ~(USBPHY_TX_D_CAL_MASK | USBPHY_TX_TXCAL45DM_MASK |
             USBPHY_TX_TXCAL45DP_MASK);
  phytx |= USBPHY_TX_D_CAL(0x0C) | USBPHY_TX_TXCAL45DP(0x06) |
           USBPHY_TX_TXCAL45DM(0x06);
  usb_phy->TX = phytx;
}

static void init_usb_hardware(void) { init_usb_instance(USB_DEVICE_INSTANCE); }

void DatoUSB::init() {
  init_usb_hardware();
  tusb_init();
}

extern "C" {
// Provide the prototypes for the interrupt handlers. The iMX RT SDK doesn't.
// The SDK only links to them from assembly.
void USB_OTG1_IRQHandler(void);
void USB_OTG1_IRQHandler(void) { usb_irq_handler(USB_DEVICE_INSTANCE); }
}
