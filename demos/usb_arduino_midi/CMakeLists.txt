CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(usb_arduino_midi-rt1011)
set(EXECUTABLE_NAME usb_arduino_midi-rt1011.elf)

set(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

include(${COMMON_CMAKE}/lib.cmake)

add_executable(${EXECUTABLE_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/usb_arduino_midi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MIDIUSB.cpp
    ${LIB_SRC_FILES}
)


include(${COMMON_CMAKE}/common.cmake)

set(TINYUSB_SRC ${CMAKE_CURRENT_LIST_DIR}/../../libraries/tinyusb/src)
set(ARDUINO_USB_MIDI_SRC ${CMAKE_CURRENT_LIST_DIR}/../../libraries/Arduino-USBMIDI/src)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_DIRS}
    ${TINYUSB_SRC}
    ${ARDUINO_USB_MIDI_SRC}
)

