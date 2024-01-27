
target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SOURCE_DIR}/usb/usb.cpp
  ${LIB_SOURCE_DIR}/usb/usb_descriptors.c
  ${LIB_SOURCE_DIR}/usb/midi_usb_bridge/MIDIUSB.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SOURCE_DIR}/usb
  ${LIB_SOURCE_DIR}/usb/midi_usb_bridge
  ${DEPS_DIR}/arduino_midi_library/src
  ${DEPS_DIR}/Arduino-USBMIDI/src
  ${DEPS_DIR}/tinyusb/src
)

include(${DEPS_DIR}/tinyusb.cmake)
include(${DEPS_DIR}/arduino_midi_library/src/component_arduino_midi_library.cmake)
