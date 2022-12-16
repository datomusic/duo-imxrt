#include "Arduino.h"
#include "fsl_pit.h"
  
extern volatile unsigned long internal_clock;

namespace InterruptTimer {
  void init (void);
  void isr (void);
  void setTimerPeriod (uint32_t usec);
}