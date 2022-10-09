SET(CMAKE_SYSTEM_NAME Generic)
ENABLE_LANGUAGE(ASM)

set(MCUX_SDK_PROJECT_NAME ${EXECUTABLE_NAME})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../)

include(${CMAKE_CURRENT_LIST_DIR}/flags.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/config.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/modules.cmake)

include(${COMMON_CMAKE}/lib.cmake)

if(WITH_USB)
  include(${COMMON_CMAKE}/usb.cmake)
endif(WITH_USB)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${LIB_INCLUDE_DIRS}
)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SRC_FILES}
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
  -Wl,--start-group m
  c
  gcc
  nosys
  -Wl,--end-group
)
