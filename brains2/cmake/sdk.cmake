
# config to select component, the format is CONFIG_USE_${component}
set(CONFIG_USE_component_serial_manager_uart_MIMXRT1011 true)
set(CONFIG_USE_driver_lpuart_MIMXRT1011 true)
set(CONFIG_USE_middleware_baremetal_MIMXRT1011 true)


set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${SDK_DIR}/devices/MIMXRT1011/drivers
    ${SDK_DIR}/devices/MIMXRT1011
    ${SDK_DIR}/devices/MIMXRT1011/utilities
    ${SDK_DIR}/components/uart
    ${SDK_DIR}/components/serial_manager
    ${SDK_DIR}/components/lists
    ${SDK_DIR}/components/osa
    ${SDK_DIR}/devices/MIMXRT1011/xip
    ${SDK_DIR}/CMSIS/Core/Include
    ${SDK_DIR}/CMSIS/DSP
    ${SDK_DIR}/xip
    ${SDK_DIR}/middleware/usb
    ${SDK_DIR}/middleware/usb/osa
)

include(driver_igpio_MIMXRT1011)
include(driver_clock_MIMXRT1011)
include(driver_common_MIMXRT1011)
include(driver_edma_MIMXRT1011)
include(driver_sai_edma_MIMXRT1011)
include(driver_pwm_MIMXRT1011)
include(driver_flexio_MIMXRT1011)
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
include(driver_xip_board_duobrains2_MIMXRT1011)
include(CMSIS_Include_core_cm_MIMXRT1011)
include(CMSIS_DSP_Library)
include(utilities_misc_utilities_MIMXRT1011)
include(device_MIMXRT1011_system_MIMXRT1011)
include(driver_adc_12b1msps_sar_MIMXRT1011)
