CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(usb_arduino_midi-rt1011)
set(EXECUTABLE_NAME usb_arduino_midi-rt1011.elf)

set(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/usb_arduino_midi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MIDIUSB.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

set(WITH_USB true)
include(${COMMON_CMAKE}/common.cmake)
