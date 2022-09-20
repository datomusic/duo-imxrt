CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(usb_midi-rt1011)
set(EXECUTABLE_NAME usb_midi-rt1011.elf)

set(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

include(${COMMON_CMAKE}/lib_src_files.cmake)

add_definitions(-DMCU_VARIANT=MIMXRT1011)
add_definitions(-DCFG_TUSB_MCU=OPT_MCU_MIMXRT)

add_executable(${EXECUTABLE_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/usb_midi.cpp
    ${LIB_SRC_FILES}
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/tinyusb/src/tusb.c
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/tinyusb/src/device/usbd.c
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/tinyusb/src/device/usbd_control.c
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/tinyusb/src/class/midi/midi_device.c
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/tinyusb/src/common/tusb_fifo.c
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/tinyusb/src/portable/nxp/khci/dcd_khci.c
)


target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/../../deps/tinyusb/src
)

include(${COMMON_CMAKE}/common.cmake)
