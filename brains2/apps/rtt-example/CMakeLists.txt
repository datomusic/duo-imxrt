CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
project(flash-writing)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME flash-writing.elf)

add_executable(${EXECUTABLE_NAME}
    main.c
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    .
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)
