SET(CMAKE_SYSTEM_NAME Generic)
ENABLE_LANGUAGE(ASM)

set(MCUX_SDK_PROJECT_NAME ${EXECUTABLE_NAME})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

set(SDK_DIR ${CMAKE_CURRENT_LIST_DIR}/../sdk/)
set(DEPS_DIR ${CMAKE_CURRENT_LIST_DIR}/../deps/)

set(COMMON_CMAKE ${CMAKE_CURRENT_LIST_DIR})

include(${COMMON_CMAKE}/flags.cmake)
include(${COMMON_CMAKE}/config.cmake)
include(${COMMON_CMAKE}/sdk.cmake)
include(${COMMON_CMAKE}/deps.cmake)
include(${COMMON_CMAKE}/lib.cmake)

if(WITH_USB)
  include(${COMMON_CMAKE}/add_git_defines.cmake)
  include(${COMMON_CMAKE}/usb.cmake)
endif(WITH_USB)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  # Disable warning about LOAD segment with RWX permissions.
  # Our data section becomes RWX because of C++ object initialization, supposedly.
  # Source: https://github.com/jsphuebner/stm32-sine/issues/24#issuecomment-1382731411
  -Wl,--no-warn-rwx-segments

  -Wl,--start-group m
  c
  gcc
  nosys
  -Wl,--end-group
)
