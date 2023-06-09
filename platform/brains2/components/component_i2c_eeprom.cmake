include_guard(GLOBAL)
message("I2C_EEPROM component is included.")

set(I2C_EEPROM_SRC ${CMAKE_CURRENT_LIST_DIR}/../libraries/I2C_EEPROM)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${I2C_EEPROM_SRC}/I2C_eeprom.cpp
)
