CMAKE_MINIMUM_REQUIRED (VERSION 3.20.0)
project(led_demo-rt1011)
ENABLE_LANGUAGE(ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SYSTEM_NAME Generic)

if(NOT DEFINED TOOLCHAIN_FILE)
  set(TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/sdk/armgcc/arm-none-eabi-gcc.cmake)
endif(NOT DEFINED TOOLCHAIN_FILE)

set(EXECUTABLE_NAME led_demo-rt1011.elf)
add_executable(${EXECUTABLE_NAME})

message("App dir: " ${APP_DIR})
message("BOARD: " ${BOARD})

if(NOT DEFINED BOARD)
  set(BOARD "DUO_BRAINS_2.1")
endif(NOT DEFINED BOARD)

add_subdirectory(${APP_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(COMMON_CMAKE ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(${CMAKE_CURRENT_LIST_DIR}/sdk/flags.cmake)
add_subdirectory(sdk)

# For apps, USB always on
set(WITH_USB true)

include(${COMMON_CMAKE}/lib.cmake)

if(WITH_USB)
  set(MCUX_SDK_PROJECT_NAME ${EXECUTABLE_NAME})
  include(${COMMON_CMAKE}/add_git_defines.cmake)
  target_compile_definitions(${EXECUTABLE_NAME} PRIVATE DATO_BOARD="${BOARD}")
  include(${COMMON_CMAKE}/usb.cmake)
endif(WITH_USB)


target_link_libraries(${EXECUTABLE_NAME} PUBLIC
  -Wl,--start-group m
  c
  gcc
  nosys
  -Wl,--end-group
)

