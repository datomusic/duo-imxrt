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



if(USB_ENABLED)
    include(
        ${COMMON_CMAKE}/../components/component_tinyusb.cmake
        ${COMMON_CMAKE}/../components/arduino_midi_library/src/component_arduino_midi_library.cmake
    )
endif(USB_ENABLED)
