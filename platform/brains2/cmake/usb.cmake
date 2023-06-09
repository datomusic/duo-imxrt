
# TinyUSB defines
add_definitions(
  -DCFG_TUSB_MCU=OPT_MCU_MIMXRT11XX
  -DCFG_TUD_MIDI_RX_BUFSIZE=512
  -DCFG_TUD_CDC_RX_BUFSIZE=512
  -DCFG_TUD_MIDI_TX_BUFSIZE=512
  -DCFG_TUD_CDC_TX_BUFSIZE=512
  -DCFG_TUD_MSC_BUFSIZE=1024
)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SOURCE_DIR}/usb/usb.cpp
  ${LIB_SOURCE_DIR}/usb/usb_descriptors.c
  ${LIB_SOURCE_DIR}/usb/midi_usb_bridge/MIDIUSB.cpp
)

set(TINYUSB_SRC ${CMAKE_CURRENT_LIST_DIR}/../libraries/tinyusb/src)
set(ARDUINO_MIDI_LIBRARY_SRC ${CMAKE_CURRENT_LIST_DIR}/../components/arduino_midi_library/src)
set(I2C_EEPROM_SRC ${CMAKE_CURRENT_LIST_DIR}/../libraries/I2C_EEPROM)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${TINYUSB_SRC}
  ${LIB_SOURCE_DIR}/usb
  ${LIB_SOURCE_DIR}/usb/midi_usb_bridge
  ${ARDUINO_MIDI_LIBRARY_SRC}
  ${I2C_EEPROM_SRC}
  ${CMAKE_CURRENT_LIST_DIR}/../libraries/Arduino-USBMIDI/src
)

include(${COMMON_CMAKE}/../components/component_tinyusb.cmake)
include(${COMMON_CMAKE}/../components/component_i2c_eeprom.cmake)
include(${COMMON_CMAKE}/../components/arduino_midi_library/src/component_arduino_midi_library.cmake)
