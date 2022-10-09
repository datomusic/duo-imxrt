CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(led_demo-rt1011)
set(EXECUTABLE_NAME led_demo-rt1011.elf)

SET(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/led_demo.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

include(${COMMON_CMAKE}/build_app.cmake)
