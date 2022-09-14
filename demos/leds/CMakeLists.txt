# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR}/../../armgcc)
SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_TYPE})

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

project(arduino-rt1011)

set(MCUX_SDK_PROJECT_NAME arduino-rt1011.elf)

include(${ROOT_DIR}/armgcc/flags.cmake)
include(${ROOT_DIR}/armgcc/config.cmake)


SET(LIB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src/lib)

file(GLOB LIB_SRC_FILES
    ${LIB_SOURCE_DIR}/*.cpp
    ${LIB_SOURCE_DIR}/*.c
)

add_executable(${MCUX_SDK_PROJECT_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/led_demo.cpp
    ${LIB_SRC_FILES}
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ROOT_DIR}/src/lib
)

set(CMAKE_MODULE_PATH
    ${ROOT_DIR}/devices/MIMXRT1011/drivers
    ${ROOT_DIR}/devices/MIMXRT1011
    ${ROOT_DIR}/devices/MIMXRT1011/utilities
    ${ROOT_DIR}/components/uart
    ${ROOT_DIR}/components/serial_manager
    ${ROOT_DIR}/components/lists
    ${ROOT_DIR}/components/osa
    ${ROOT_DIR}/devices/MIMXRT1011/xip
    ${ROOT_DIR}/CMSIS/Core/Include
    ${ROOT_DIR}/CMSIS/DSP
    ${ROOT_DIR}/xip
    ${ROOT_DIR}/components/Arduino
    ${ROOT_DIR}/middleware/usb
    ${ROOT_DIR}/middleware/usb/osa
)

# include modules
include(driver_igpio_MIMXRT1011)

include(driver_clock_MIMXRT1011)

include(driver_common_MIMXRT1011)

include(driver_edma_MIMXRT1011)

include(device_MIMXRT1011_CMSIS_MIMXRT1011)

include(utility_debug_console_MIMXRT1011)

include(component_lpuart_adapter_MIMXRT1011)

include(component_serial_manager_MIMXRT1011)

include(component_lists_MIMXRT1011)

include(component_serial_manager_uart_MIMXRT1011)

include(driver_lpuart_MIMXRT1011)

include(device_MIMXRT1011_startup_MIMXRT1011)

include(driver_iomuxc_MIMXRT1011)

include(utility_assert_MIMXRT1011)

include(driver_xip_device_MIMXRT1011)

include(driver_xip_board_evkmimxrt1010_MIMXRT1011)

include(CMSIS_Include_core_cm_MIMXRT1011)

include(CMSIS_DSP_Library)

include(utilities_misc_utilities_MIMXRT1011)

include(device_MIMXRT1011_system_MIMXRT1011)

include(driver_adc_12b1msps_sar_MIMXRT1011)

include(component_Arduino_MIMXRT1011)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)
