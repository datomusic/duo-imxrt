#ifndef MUSIN_FILESYSTEM_FLASH_GUARD_H_
#define MUSIN_FILESYSTEM_FLASH_GUARD_H_

namespace musin::filesystem {

/**
 * @brief Policy hook run around every flash erase/program of the data
 * filesystem's block device.
 *
 * Portable contract: flash writes must not silence the audio interrupt.
 * Each port decides how to honour (or consciously relax) that. The Pico
 * port masks all interrupts below the audio DMA priority so the
 * RAM-resident audio render keeps running while flash is busy; a first
 * port on another platform may simply disable interrupts and accept the
 * glitch.
 *
 * enter() is called immediately before an erase/program operation and
 * exit() immediately after. Implementations must tolerate being invoked
 * from non-interrupt context only, and enter()/exit() are never nested.
 */
class FlashGuard {
public:
  virtual void enter() = 0;
  virtual void exit() = 0;

protected:
  ~FlashGuard() = default;
};

} // namespace musin::filesystem

#endif // MUSIN_FILESYSTEM_FLASH_GUARD_H_
