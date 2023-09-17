#ifndef CHANNEL_H_QEB6Q1AE
#define CHANNEL_H_QEB6Q1AE

#include "fsl_edma.h"
#include <stdint.h>

#define DMA_SIGNAL_ID 0

struct Channel {
  void disable() { DMA0->CERQ = DMA_SIGNAL_ID; }
  void enable() { DMA0->SERQ = DMA_SIGNAL_ID; }
  void set_disable_on_completion(bool b) { DMA0->TCD->CSR = DMA_CSR_DREQ(b); }

  void set_source_linear_buffer(uint32_t *buffer, uint32_t buf_len) {
    // set source address
    DMA0->TCD->SADDR = (uint32_t)buffer;
    // set source offset
    DMA0->TCD->SOFF = sizeof(uint32_t);
    // set source attributes
    DMA0->TCD->ATTR |=
        DMA_ATTR_SMOD(0) | DMA_ATTR_SSIZE(2); // TCD Data transfer ID for u32

    // set source last address adjustment
    DMA0->TCD->SLAST = (buf_len * sizeof(uint32_t));
  }

  void set_destination_hardware(volatile uint32_t *destination_address) {
    // chan.set_destination_address(hardware_destination);
    DMA0->TCD->DADDR = (uint32_t)destination_address;

    // chan.set_destination_offset(0);
    DMA0->TCD->DOFF = 0;

    // chan.set_destination_attributes::<E>(0);
    DMA0->TCD->ATTR |=
        DMA_ATTR_DMOD(0) | DMA_ATTR_DSIZE(2); // TCD Data transfer ID for u32

    // chan.set_destination_last_address_adjustment(0);
    DMA0->TCD->DLAST_SGA = 0;
  }

  void set_minor_loop_bytes(uint32_t count) { DMA0->TCD->NBYTES_MLNO = count; }

  void set_transfer_iterations(uint16_t count) {
    DMA0->TCD->CITER_ELINKNO = count;
    DMA0->TCD->BITER_ELINKNO = count;
  }

  void enable_destination() {}
};

#endif /* end of include guard: CHANNEL_H_QEB6Q1AE */
