#ifndef ARDUINO_STUBS_H_SIADU43M
#define ARDUINO_STUBS_H_SIADU43M

int touchRead(int8_t pin) {
  // TODO
  return 0;
}

int eeprom_read_byte(int _) {
  // TODO
  return 0;
}

int eeprom_write_byte(int _a, int _b) {
  // TODO
  return 0;
}

// These go in avr/pgmspace.h
#ifndef DMAMEM
#define DMAMEM __attribute__ ((section(".m_data2"), used))
#endif
#ifndef FASTRUN
#define FASTRUN __attribute__ ((section(".m_qacode") ))
#endif
#ifndef PROGMEM
#define PROGMEM __attribute__((section(".m_text")))
#endif
#ifndef FLASHMEM
#define FLASHMEM //__attribute__((section(".flashmem")))
#endif
#define EXTMEM //__attribute__((section(".externalram")))

#endif /* end of include guard: ARDUINO_STUBS_H_SIADU43M */
