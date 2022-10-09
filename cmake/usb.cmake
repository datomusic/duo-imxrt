
# TinyUSB defines
add_definitions(
  -DCFG_TUSB_MCU=OPT_MCU_MIMXRT11XX
  -DCFG_TUD_MIDI_RX_BUFSIZE=512
  -DCFG_TUD_CDC_RX_BUFSIZE=512
  -DCFG_TUD_MIDI_TX_BUFSIZE=512
  -DCFG_TUD_CDC_TX_BUFSIZE=512
  -DCFG_TUD_MSC_BUFSIZE=1024
)

set(LIB_SRC_FILES
  ${LIB_SRC_FILES}
  ${LIB_SOURCE_DIR}/usb/usb.cpp
  ${LIB_SOURCE_DIR}/usb/usb_descriptors.c
)

set(TINYUSB_SRC ${CMAKE_CURRENT_LIST_DIR}/../libraries/tinyusb/src)
set(ARDUINO_MIDI_LIBRARY_SRC ${CMAKE_CURRENT_LIST_DIR}/../components/arduino_midi_library/src)

set(LIB_INCLUDE_DIRS
  ${LIB_INCLUDE_DIRS}
  ${TINYUSB_SRC}
  ${LIB_SOURCE_DIR}/usb
  ${ARDUINO_MIDI_LIBRARY_SRC}
  ${CMAKE_CURRENT_LIST_DIR}/../libraries/Arduino-USBMIDI/src
)

include(${COMMON_CMAKE}/../components/component_tinyusb.cmake)
include(${COMMON_CMAKE}/../components/arduino_midi_library/src/component_arduino_midi_library.cmake)
