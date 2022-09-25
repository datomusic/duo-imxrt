CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(usb_midi-rt1011)
set(EXECUTABLE_NAME usb_midi-rt1011.elf)

set(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

include(${COMMON_CMAKE}/lib_src_files.cmake)

add_definitions(-DMCU_VARIANT=MIMXRT1011)

# TinyUSB defines
add_definitions(-DCFG_TUSB_MCU=OPT_MCU_MIMXRT10XX -DCFG_TUD_MIDI_RX_BUFSIZE=512 -DCFG_TUD_CDC_RX_BUFSIZE=512 -DCFG_TUD_MIDI_TX_BUFSIZE=512 -DCFG_TUD_CDC_TX_BUFSIZE=512 -DCFG_TUD_MSC_BUFSIZE=1024)

set(TUSB_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../libraries/tinyusb)
set(TUSB_SRC ${TUSB_ROOT}/src)
set(TUSB_HW ${TUSB_ROOT}/hw)

add_executable(${EXECUTABLE_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/usb_midi.cpp
    ${LIB_SRC_FILES}
    ${TUSB_SRC}/class/cdc/cdc_device.c
    ${TUSB_SRC}/common/tusb_fifo.c
    ${TUSB_SRC}/device/usbd.c
    ${TUSB_SRC}/device/usbd_control.c
    ${TUSB_SRC}/tusb.c
    ${TUSB_SRC}/class/midi/midi_device.c
    ${TUSB_SRC}/portable/chipidea/ci_hs/dcd_ci_hs.c
)


include(${COMMON_CMAKE}/common.cmake)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_DIRS}
    ${TUSB_SRC}
)

