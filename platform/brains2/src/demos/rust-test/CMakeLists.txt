CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(rust-test)
set(EXECUTABLE_NAME rust-test.elf)


add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/rust_test.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

find_library(LIB_RUST librust_lib.a ${CMAKE_CURRENT_SOURCE_DIR}/rust-lib/target/thumbv7em-none-eabihf/release)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  ${LIB_RUST}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/build_app.cmake)
