CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
project(led_demo-rt1011)
ENABLE_LANGUAGE(ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SYSTEM_NAME Generic)

set(EXECUTABLE_NAME led_demo-rt1011.elf)
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/apps/demos/leds/)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(COMMON_CMAKE ${CMAKE_CURRENT_LIST_DIR}/cmake)

add_executable(${EXECUTABLE_NAME}
)

include(${CMAKE_CURRENT_LIST_DIR}/sdk/flags.cmake)
add_subdirectory(sdk)
include(${COMMON_CMAKE}/lib.cmake)

if(WITH_USB)
  include(${COMMON_CMAKE}/add_git_defines.cmake)
  include(${COMMON_CMAKE}/usb.cmake)
endif(WITH_USB)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
  -Wl,--start-group m
  c
  gcc
  nosys
  -Wl,--end-group
)

add_subdirectory(${APP_DIR})
