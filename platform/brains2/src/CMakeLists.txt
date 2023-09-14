CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duo)

ENABLE_LANGUAGE(ASM)

set(EXECUTABLE_NAME duo-imxrt.elf)

if(NOT DEFINED BOARD)
    set(BOARD "DUO_BRAINS_2.1")
endif(NOT DEFINED BOARD)
add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/tempo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/fast_led/FastLED.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../firmware/effect_custom_envelope.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/boards/${BOARD}/pins.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/boards/${BOARD}/audio.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../../
    ${CMAKE_CURRENT_SOURCE_DIR}/duo/boards/${BOARD}
    ${CMAKE_CURRENT_LIST_DIR}/../libraries/
    ${CMAKE_CURRENT_LIST_DIR}/adapters/fast_led
)

set(WITH_USB true)
add_definitions("-DDATO_BOARD=\"${BOARD}\"")

find_library(LIB_RUST librust_lib.a ${CMAKE_CURRENT_SOURCE_DIR}/rust-lib/target/thumbv7em-none-eabihf/release)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  ${LIB_RUST}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/build_app.cmake)

