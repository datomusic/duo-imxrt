
set(CORE_DIR ${CMAKE_CURRENT_LIST_DIR}/../core/)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${CORE_DIR}
  ${CORE_DIR}/custom_teensy_audio
  ${CORE_DIR}/arduino_compat
  ${CORE_DIR}/adapters/fast_led

  ${CORE_DIR}/boards/${BOARD}
)

file(GLOB LIB_SRC_FILES
  ${CORE_DIR}/lib/*.cpp
  ${CORE_DIR}/lib/*.c
)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SRC_FILES}
  ${CORE_DIR}/syscall_stubs.c
  ${CORE_DIR}/custom_teensy_audio/AudioStream.cpp
  ${CORE_DIR}/custom_teensy_audio/output_mqs.cpp
  ${CORE_DIR}/custom_teensy_audio/output_pt8211.cpp
  ${CORE_DIR}/adapters/fast_led/FastLED.cpp

  ${CORE_DIR}/boards/${BOARD}/pins.cpp
  ${CORE_DIR}/boards/${BOARD}/audio.cpp
)
