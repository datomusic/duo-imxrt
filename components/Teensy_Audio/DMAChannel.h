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

  /***************************************/
  /**    Data Transfer                  **/
  /***************************************/

  // Use a single variable as the data source.  Typically a register
  // for receiving data from one of the hardware peripherals is used.
  void source(volatile const signed char &p) {
    source(*(volatile const uint8_t *)&p);
  }
  void source(volatile const unsigned char &p) {}
  void source(volatile const signed short &p) {
    source(*(volatile const uint16_t *)&p);
  }
  void source(volatile const unsigned short &p) {}
  void source(volatile const signed int &p) {
    source(*(volatile const uint32_t *)&p);
  }
  void source(volatile const unsigned int &p) {
    source(*(volatile const uint32_t *)&p);
  }
  void source(volatile const signed long &p) {
    source(*(volatile const uint32_t *)&p);
  }
  void source(volatile const unsigned long &p) {}

  // Use a buffer (array of data) as the data source.  Typically a
  // buffer for transmitting data is used.
  void sourceBuffer(volatile const signed char p[], unsigned int len) {
    sourceBuffer((volatile const uint8_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned char p[], unsigned int len) {}
  void sourceBuffer(volatile const signed short p[], unsigned int len) {
    sourceBuffer((volatile const uint16_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned short p[], unsigned int len) {}
  void sourceBuffer(volatile const signed int p[], unsigned int len) {
    sourceBuffer((volatile const uint32_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned int p[], unsigned int len) {
    sourceBuffer((volatile const uint32_t *)p, len);
  }
  void sourceBuffer(volatile const signed long p[], unsigned int len) {
    sourceBuffer((volatile const uint32_t *)p, len);
  }
  void sourceBuffer(volatile const unsigned long p[], unsigned int len) {}

  // Use a circular buffer as the data source
  void sourceCircular(volatile const signed char p[], unsigned int len) {
    sourceCircular((volatile const uint8_t *)p, len);
  }
  void sourceCircular(volatile const unsigned char p[], unsigned int len) {}
  void sourceCircular(volatile const signed short p[], unsigned int len) {
    sourceCircular((volatile const uint16_t *)p, len);
  }
  void sourceCircular(volatile const unsigned short p[], unsigned int len) {}
  void sourceCircular(volatile const signed int p[], unsigned int len) {
    sourceCircular((volatile const uint32_t *)p, len);
  }
  void sourceCircular(volatile const unsigned int p[], unsigned int len) {
    sourceCircular((volatile const uint32_t *)p, len);
  }
  void sourceCircular(volatile const signed long p[], unsigned int len) {
    sourceCircular((volatile const uint32_t *)p, len);
  }
  void sourceCircular(volatile const unsigned long p[], unsigned int len) {}

  // Use a single variable as the data destination.  Typically a register
  // for transmitting data to one of the hardware peripherals is used.
  void destination(volatile signed char &p) {
    destination(*(volatile uint8_t *)&p);
  }
  void destination(volatile unsigned char &p) {}
  void destination(volatile signed short &p) {
    destination(*(volatile uint16_t *)&p);
  }
  void destination(volatile unsigned short &p) {}
  void destination(volatile signed int &p) {}
  void destination(volatile unsigned int &p) {}
  void destination(volatile signed long &p) {}
  void destination(volatile unsigned long &p) {}

  // Use a buffer (array of data) as the data destination.  Typically a
  // buffer for receiving data is used.
  void destinationBuffer(volatile signed char p[], unsigned int len) {}
  void destinationBuffer(volatile unsigned char p[], unsigned int len) {}
  void destinationBuffer(volatile signed short p[], unsigned int len) {
    destinationBuffer((volatile uint16_t *)p, len);
  }
  void destinationBuffer(volatile unsigned short p[], unsigned int len) {}
  void destinationBuffer(volatile signed int p[], unsigned int len) {}
  void destinationBuffer(volatile unsigned int p[], unsigned int len) {}
  void destinationBuffer(volatile signed long p[], unsigned int len) {}
  void destinationBuffer(volatile unsigned long p[], unsigned int len) {}

  // Use a circular buffer as the data destination
  void destinationCircular(volatile signed char p[], unsigned int len) {}
  void destinationCircular(volatile unsigned char p[], unsigned int len) {}
  void destinationCircular(volatile signed short p[], unsigned int len) {}
  void destinationCircular(volatile unsigned short p[], unsigned int len) {}
  void destinationCircular(volatile signed int p[], unsigned int len) {}
  void destinationCircular(volatile unsigned int p[], unsigned int len) {}
  void destinationCircular(volatile signed long p[], unsigned int len) {}
  void destinationCircular(volatile unsigned long p[], unsigned int len) {}

  /*************************************************/
  /**    Quantity of Data to Transfer             **/
  /*************************************************/

  // Set the data size used for each triggered transfer
  void transferSize(unsigned int len) {}

  // Set the number of transfers (number of triggers until complete)
  void transferCount(unsigned int len) {}

  /*************************************************/
  /**    Special Options / Features               **/
  /*************************************************/

  void interruptAtCompletion(void) {}

  void interruptAtHalf(void) {}

  void disableOnCompletion(void) {}

  void replaceSettingsOnCompletion(const DMABaseClass &settings) {}

protected:
  // users should not be able to create instances of DMABaseClass, which
  // require the inheriting class to initialize the TCD pointer.
  DMABaseClass() {}

  static inline void copy_tcd(TCD_t *dst, const TCD_t *src) {}
};

// DMASetting represents settings stored only in memory, which can be
// applied to any DMA channel.

class DMASetting : public DMABaseClass {
public:
  DMASetting() { TCD = &tcddata; }
  DMASetting(const DMASetting &c) {
    TCD = &tcddata;
    *this = c;
  }
  DMASetting(const DMABaseClass &c) {
    TCD = &tcddata;
    *this = c;
  }
  DMASetting &operator=(const DMABaseClass &rhs) {
    copy_tcd(TCD, rhs.TCD);
    return *this;
  }

private:
  TCD_t tcddata __attribute__((aligned(32)));
};

// DMAChannel reprents an actual DMA channel and its current settings

class DMAChannel : public DMABaseClass {
public:
  /*************************************************/
  /**    Channel Allocation                       **/
  /*************************************************/

  DMAChannel() { begin(); }
  DMAChannel(const DMAChannel &c) {}
  DMAChannel(const DMASetting &c) {}
  DMAChannel(bool allocate) {}
  DMAChannel &operator=(const DMAChannel &rhs) { return *this; }
  DMAChannel &operator=(const DMASetting &rhs) { return *this; }
  ~DMAChannel() { release(); }
  void begin(bool force_initialization = false);

private:
  void release(void);

public:
  /***************************************/
  /**    Triggering                     **/
  /***************************************/

  // Triggers cause the DMA channel to actually move data.  Each
  // trigger moves a single data unit, which is typically 8, 16 or
  // 32 bits.  If a channel is configured for 200 transfers

  // Use a hardware trigger to make the DMA channel run
  void triggerAtHardwareEvent(uint8_t source) {}

  // Use another DMA channel as the trigger, causing this
  // channel to trigger after each transfer is makes, except
  // the its last transfer.  This effectively makes the 2
  // channels run in parallel until the last transfer
  void triggerAtTransfersOf(DMABaseClass &ch) {}

  // Use another DMA channel as the trigger, causing this
  // channel to trigger when the other channel completes.
  void triggerAtCompletionOf(DMABaseClass &ch) {}

  // Cause this DMA channel to be continuously triggered, so
  // it will move data as rapidly as possible, without waiting.
  // Normally this would be used with disableOnCompletion().
  void triggerContinuously(void) {}

  // Manually trigger the DMA channel.
  void triggerManual(void) {}

  /***************************************/
  /**    Interrupts                     **/
  /***************************************/

  // An interrupt routine can be run when the DMA channel completes
  // the entire transfer, and also optionally when half of the
  // transfer is completed.
  void attachInterrupt(void (*isr)(void)) {}

  void attachInterrupt(void (*isr)(void), uint8_t prio) {}

  void detachInterrupt(void) {}

  void clearInterrupt(void) {}

  /***************************************/
  /**    Enable / Disable               **/
  /***************************************/

  void enable(void) {}
  void disable(void) {}

  /***************************************/
  /**    Status                         **/
  /***************************************/

  bool complete(void) { return false; }
  void clearComplete(void) {}
  bool error(void) { return false; }
  void clearError(void) {}
  void *sourceAddress(void) { return 0; }
  void *destinationAddress(void) { return 0; }

  /***************************************/
  /**    Direct Hardware Access         **/
  /***************************************/

  // For complex and unusual configurations not possible with the above
  // functions, the Transfer Control Descriptor (TCD) and channel number
  // can be used directly.  This leads to less portable and less readable
  // code, but direct control of all parameters is possible.
  uint8_t channel;
  // TCD is accessible due to inheritance from DMABaseClass
};

// arrange the relative priority of 2 or more DMA channels
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2);
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3);
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3,
                      DMAChannel &ch4);

#endif // DMAChannel_h_
