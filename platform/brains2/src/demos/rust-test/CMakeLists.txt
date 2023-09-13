CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(rust-test)
set(EXECUTABLE_NAME rust-test.elf)


add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/rust_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fsl_flexio.c
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

include(ExternalProject)

ExternalProject_Add(
 rust-lib
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND cargo build --release  --target=thumbv7em-none-eabihf
  BINARY_DIR "${CMAKE_SOURCE_DIR}/rust-lib"
  INSTALL_COMMAND ""
  LOG_BUILD ON
)

add_dependencies(${EXECUTABLE_NAME} rust-lib)

find_library(LIB_RUST librust_lib.a ${CMAKE_CURRENT_SOURCE_DIR}/rust-lib/target/thumbv7em-none-eabihf/release)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/rust-lib/target/thumbv7em-none-eabihf/release/librust_lib.a
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/build_app.cmake)
