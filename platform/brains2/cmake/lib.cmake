
target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../src/
  ${CMAKE_CURRENT_LIST_DIR}/../src/lib/custom_teensy_audio
  ${CMAKE_CURRENT_LIST_DIR}/../src/arduino_compat
)

set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../src/lib)

file(GLOB LIB_SRC_FILES
  ${LIB_SOURCE_DIR}/*.cpp
  ${LIB_SOURCE_DIR}/*.c
)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SRC_FILES}
  ${CMAKE_CURRENT_LIST_DIR}/../devices/MIMXRT1011/drivers/fsl_flexio.c
  ${LIB_SOURCE_DIR}/custom_teensy_audio/AudioStream.cpp
  ${LIB_SOURCE_DIR}/custom_teensy_audio/output_mqs.cpp
  ${LIB_SOURCE_DIR}/custom_teensy_audio/output_pt8211.cpp
)
