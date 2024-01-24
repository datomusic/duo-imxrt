target_sources(${EXECUTABLE_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tempo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../core/adapters/fast_led/FastLED.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/effect_custom_envelope.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/seq.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}/pins.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}/audio.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../../../
  ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
  ${CMAKE_CURRENT_LIST_DIR}/../libraries/
  ${CMAKE_CURRENT_LIST_DIR}/../../core/adapters/fast_led
)
