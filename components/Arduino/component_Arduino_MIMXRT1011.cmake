include_guard(GLOBAL)
message("Arduino component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/hooks.c
    ${CMAKE_CURRENT_LIST_DIR}/wiring.c
    ${CMAKE_CURRENT_LIST_DIR}/wiring_analog.c
    ${CMAKE_CURRENT_LIST_DIR}/wiring_digital.c
    ${CMAKE_CURRENT_LIST_DIR}/Uart.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Print.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/USBCDC.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_common_MIMXRT1011)
