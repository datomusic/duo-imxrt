CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duobrains2)

ENABLE_LANGUAGE(ASM)

set(EXECUTABLE_NAME duobrains2.elf)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/duo2/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/adapters/FastLED.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_DIRS}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)
