#include "eeprom.h"

I2C_eeprom eeprom(DEVICEADDRESS,I2C_DEVICESIZE_24LC02, &Wire1);

bool eeprom_init() {
  return eeprom.begin();
}

uint8_t eeprom_read_byte(const uint8_t addr) {
  return eeprom.readByte(addr);
}



void eeprom_write_byte(const uint8_t addr, uint8_t value) {
  eeprom.writeByte(addr, value);
}
