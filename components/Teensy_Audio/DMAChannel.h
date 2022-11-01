#ifndef DMAChannel_h_
#define DMAChannel_h_

#include <stdint.h>

#define DMACHANNEL_HAS_BEGIN
#define DMACHANNEL_HAS_BOOLEAN_CTOR

class DMABaseClass {
public:
  typedef struct __attribute__((packed, aligned(4))) {
    volatile const void *volatile SADDR;
    int16_t SOFF;
    union {
      uint16_t ATTR;
      struct {
        uint8_t ATTR_DST;
        uint8_t ATTR_SRC;
      };
    };
    union {
      uint32_t NBYTES;
      uint32_t NBYTES_MLNO;
      uint32_t NBYTES_MLOFFNO;
      uint32_t NBYTES_MLOFFYES;
    };
    int32_t SLAST;
    volatile void *volatile DADDR;
    int16_t DOFF;
    union {
      volatile uint16_t CITER;
      volatile uint16_t CITER_ELINKYES;
      volatile uint16_t CITER_ELINKNO;
    };
    int32_t DLASTSGA;
    volatile uint16_t CSR;
    union {
      volatile uint16_t BITER;
      volatile uint16_t BITER_ELINKYES;
      volatile uint16_t BITER_ELINKNO;
    };
  } TCD_t;
  TCD_t *TCD;

protected:
  // users should not be able to create instances of DMABaseClass, which
  // require the inheriting class to initialize the TCD pointer.
  DMABaseClass() {}

  static inline void copy_tcd(TCD_t *dst, const TCD_t *src) {}
};

// DMAChannel reprents an actual DMA channel and its current settings

class DMAChannel : public DMABaseClass {
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
