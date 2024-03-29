SET(CMAKE_SYSTEM_NAME Generic)
ENABLE_LANGUAGE(ASM)

set(MCUX_SDK_PROJECT_NAME ${EXECUTABLE_NAME})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

set(SDK_DIR ${CMAKE_CURRENT_LIST_DIR}/../sdk/)
set(LIBRARIES_DIR ${CMAKE_CURRENT_LIST_DIR}/../libraries/)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_LIST_DIR}
)


include(dato_board)
include(flags)
include(sdk)
include(libraries)
include(core)
include(app_support)

# All apps build with USB support for now,
# since we rely on USB-MIDI to enter bootloader.
set(WITH_USB true)

if(WITH_USB)
  include(add_git_defines)
  include(usb)
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
