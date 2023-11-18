
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

set(TINYUSB_SRC ${CMAKE_CURRENT_LIST_DIR}/../../modules/tinyusb/src)
set(ARDUINO_MIDI_LIBRARY_SRC ${CMAKE_CURRENT_LIST_DIR}/../../modules/arduino_midi_library/src)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${TINYUSB_SRC}
  ${LIB_SOURCE_DIR}/usb
  ${LIB_SOURCE_DIR}/usb/midi_usb_bridge
  ${ARDUINO_MIDI_LIBRARY_SRC}
  ${CMAKE_CURRENT_LIST_DIR}/../../modules/Arduino-USBMIDI/src
)

include(${COMMON_CMAKE}/../components/component_tinyusb.cmake)
include(${COMMON_CMAKE}/../components/arduino_midi_library/src/component_arduino_midi_library.cmake)
