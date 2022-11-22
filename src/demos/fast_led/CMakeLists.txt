CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(fast_led_demo-rt1011)
set(EXECUTABLE_NAME fast_led_demo-rt1011.elf)


add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/fast_led_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../duo-imxrt/adapters/fast_led/FastLED.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../duo-imxrt/adapters/fast_led/
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/build_app.cmake)
