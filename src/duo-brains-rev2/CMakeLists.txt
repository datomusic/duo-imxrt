CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duo-brains-rev2)

ENABLE_LANGUAGE(ASM)

set(EXECUTABLE_NAME duo-brains-rev2.elf)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/fast_led/FastLED.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../duo-firmware/src/effect_custom_envelope.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../../
    ${CMAKE_CURRENT_LIST_DIR}/../../../
    ${CMAKE_CURRENT_LIST_DIR}/adapters/fast_led
)

set(WITH_USB true)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)
