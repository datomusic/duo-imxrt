CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(teensy_audio_demo-rt1011)
set(EXECUTABLE_NAME teensy_audio_demo-rt1011.elf)

SET(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

include(${COMMON_CMAKE}/lib_src_files.cmake)

add_executable(${EXECUTABLE_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/teensy_audio.cpp
    ${LIB_SRC_FILES}
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_DIRS}
)

include(${COMMON_CMAKE}/common.cmake)
