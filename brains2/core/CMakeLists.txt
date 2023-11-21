add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/adapters/fast_led/FastLED.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../shared/duo/effect_custom_envelope.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../shared/duo/seq.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# file(GLOB LIB_SRC_FILES
#   ${CMAKE_CURRENT_LIST_DIR}/lib/*.cpp
#   ${CMAKE_CURRENT_LIST_DIR}/lib/*.c
# )
