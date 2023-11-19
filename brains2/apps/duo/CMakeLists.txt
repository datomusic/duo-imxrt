CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duo)

ENABLE_LANGUAGE(ASM)

set(EXECUTABLE_NAME duo-imxrt.elf)

if(NOT DEFINED BOARD)
    set(BOARD "DUO_BRAINS_2.1")
endif(NOT DEFINED BOARD)
add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tempo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/adapters/fast_led/FastLED.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/effect_custom_envelope.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/seq.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}/pins.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}/audio.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
    ${CMAKE_CURRENT_LIST_DIR}/../libraries/
    ${CMAKE_CURRENT_LIST_DIR}/../../core/adapters/fast_led
)

set(WITH_USB true)
add_definitions("-DDATO_BOARD=\"${BOARD}\"")
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)

