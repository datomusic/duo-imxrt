namespace InterruptTimer {
  void init (void);
  void isr (void);
  void setTimerPeriod (uint32_t usec);
  volatile unsigned long _interruptCount;
  unsigned long getInterruptCount (void);
  
  volatile uint8_t polarity = HIGH; // Only used for benchmarking
}