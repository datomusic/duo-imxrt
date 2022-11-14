CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duo-imxrt)

ENABLE_LANGUAGE(ASM)

set(EXECUTABLE_NAME duo-imxrt.elf)

if(NOT DEFINED DUO_BOARD)
    set(DUO_BOARD "2_0-rc1")
endif(NOT DEFINED DUO_BOARD)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/fast_led/FastLED.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../libraries/duo-firmware/src/effect_custom_envelope.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/board/${DUO_BOARD}/pins.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/board/${DUO_BOARD}
    ${CMAKE_CURRENT_LIST_DIR}/../../libraries/
    ${CMAKE_CURRENT_LIST_DIR}/adapters/fast_led
)

set(WITH_USB true)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)
