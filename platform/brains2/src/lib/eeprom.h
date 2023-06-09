#pragma once

#include <I2C_eeprom.h>

// the address of your EEPROM
#define DEVICEADDRESS (0xA0)

bool eeprom_init();
uint8_t eeprom_read_byte(const uint8_t addr);
void eeprom_write_byte(const uint8_t addr, uint8_t value);
