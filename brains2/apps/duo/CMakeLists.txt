CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duo)

ENABLE_LANGUAGE(ASM)

set(EXECUTABLE_NAME duo-imxrt.elf)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tempo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/adapters/fast_led/FastLED.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/effect_custom_envelope.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/seq.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../../
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
    ${CMAKE_CURRENT_LIST_DIR}/../libraries/
    ${CMAKE_CURRENT_LIST_DIR}/../../core/adapters/fast_led
)

set(WITH_USB true)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
)

