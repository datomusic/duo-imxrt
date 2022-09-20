CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(led_demo-rt1011)
set(EXECUTABLE_NAME led_demo-rt1011.elf)

SET(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

include(${COMMON_CMAKE}/lib_src_files.cmake)

add_executable(${EXECUTABLE_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/led_demo.cpp
    ${LIB_SRC_FILES}
)

include(${COMMON_CMAKE}/common.cmake)
