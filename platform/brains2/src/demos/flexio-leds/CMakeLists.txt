CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(flexio-leds)
set(EXECUTABLE_NAME flexio-leds.elf)


add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/flexio_leds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsl_flexio.c
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/build_app.cmake)
