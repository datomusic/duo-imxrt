
set(USB_DIR ${CMAKE_CURRENT_LIST_DIR}/../core/lib/usb/)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${USB_DIR}/usb.cpp
  ${USB_DIR}/usb_descriptors.c
  ${USB_DIR}/midi_usb_bridge/MIDIUSB.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${USB_DIR}
  ${USB_DIR}/midi_usb_bridge
  ${LIBRARIES_DIR}/arduino_midi_library/src
  ${LIBRARIES_DIR}/Arduino-USBMIDI/src
  ${LIBRARIES_DIR}/tinyusb/src
)

include(${LIBRARIES_DIR}/tinyusb.cmake)
include(${LIBRARIES_DIR}/arduino_midi_library/src/component_arduino_midi_library.cmake)
