CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(synth_demo-rt1011)
set(EXECUTABLE_NAME synth_demo-rt1011.elf)

SET(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/synth_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../libraries/duo-firmware/src/effect_custom_envelope.cpp
    ${LIB_SRC_FILES}
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../libraries
    ${LIB_INCLUDE_DIRS}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)
