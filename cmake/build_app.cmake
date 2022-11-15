SET(CMAKE_SYSTEM_NAME Generic)
ENABLE_LANGUAGE(ASM)

set(MCUX_SDK_PROJECT_NAME ${EXECUTABLE_NAME})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../)

set(COMMON_CMAKE ${CMAKE_CURRENT_LIST_DIR})

include(${COMMON_CMAKE}/flags.cmake)
include(${COMMON_CMAKE}/config.cmake)
include(${COMMON_CMAKE}/modules.cmake)
include(${COMMON_CMAKE}/lib.cmake)

add_definitions(-DENABLE_RAM_VECTOR_TABLE=1)

if(WITH_USB)
  include(${COMMON_CMAKE}/usb.cmake)
endif(WITH_USB)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  -Wl,--start-group m
  c
  gcc
  nosys
  -Wl,--end-group
)
