CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
project(test_app)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME test_app.elf)

add_executable(${EXECUTABLE_NAME}
    main.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)
