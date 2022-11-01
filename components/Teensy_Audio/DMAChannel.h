#ifndef DMAChannel_h_
#define DMAChannel_h_

#include <stdint.h>

#define DMACHANNEL_HAS_BEGIN
#define DMACHANNEL_HAS_BOOLEAN_CTOR

// DMAChannel reprents an actual DMA channel and its current settings

class DMAChannel {
public:
  /*************************************************/
  /**    Channel Allocation                       **/
  /*************************************************/

  DMAChannel() { begin(); }
  DMAChannel(bool allocate) {}
  ~DMAChannel() { release(); }
  void begin(bool force_initialization = false);

private:
  DMAChannel(const DMAChannel &c) {}
  DMAChannel &operator=(const DMAChannel &rhs) { return *this; }
  void release(void);

public:
  // An interrupt routine can be run when the DMA channel completes
  // the entire transfer, and also optionally when half of the
  // transfer is completed.
  void attachInterrupt(void (*isr)(void)) {}

  /***************************************/
  /**    Enable / Disable               **/
  /***************************************/

  void enable(void) {}
  void disable(void) {}

  // For complex and unusual configurations not possible with the above
  // functions, the Transfer Control Descriptor (TCD) and channel number
  // can be used directly.  This leads to less portable and less readable
  // code, but direct control of all parameters is possible.
  uint8_t channel;
  // TCD is accessible due to inheritance from DMABaseClass
};

#endif // DMAChannel_h_
