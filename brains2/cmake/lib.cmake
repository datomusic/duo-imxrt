
set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../core/lib)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../core/
  ${CMAKE_CURRENT_LIST_DIR}/../core/lib/custom_teensy_audio
  ${CMAKE_CURRENT_LIST_DIR}/../core/arduino_compat
)


file(GLOB LIB_SRC_FILES
  ${LIB_SOURCE_DIR}/*.cpp
  ${LIB_SOURCE_DIR}/*.c
)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SRC_FILES}
  ${LIB_SOURCE_DIR}/custom_teensy_audio/AudioStream.cpp
  ${LIB_SOURCE_DIR}/custom_teensy_audio/output_mqs.cpp
  ${LIB_SOURCE_DIR}/custom_teensy_audio/output_pt8211.cpp
)
