set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib)

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

# target_sources(${EXECUTABLE_NAME} PRIVATE
#     ${CMAKE_CURRENT_LIST_DIR}/adapters/fast_led/FastLED.cpp
#     ${CMAKE_CURRENT_LIST_DIR}/../../shared/duo/effect_custom_envelope.cpp
#     ${CMAKE_CURRENT_LIST_DIR}/../../shared/duo/seq.cpp
# )

if(WITH_USB)
  # TinyUSB defines
add_definitions(
  -DCFG_TUSB_MCU=OPT_MCU_MIMXRT11XX
  -DCFG_TUD_MIDI_RX_BUFSIZE=512
  -DCFG_TUD_CDC_RX_BUFSIZE=512
  -DCFG_TUD_MIDI_TX_BUFSIZE=512
  -DCFG_TUD_CDC_TX_BUFSIZE=512
  -DCFG_TUD_MSC_BUFSIZE=1024
)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SOURCE_DIR}/usb/usb.cpp
  ${LIB_SOURCE_DIR}/usb/usb_descriptors.c
  ${LIB_SOURCE_DIR}/usb/midi_usb_bridge/MIDIUSB.cpp
)

set(TINYUSB_SRC ${CMAKE_CURRENT_LIST_DIR}/../../modules/tinyusb/src)
set(ARDUINO_MIDI_LIBRARY_SRC ${CMAKE_CURRENT_LIST_DIR}/../../modules/arduino_midi_library/src)

# target_include_directories(${EXECUTABLE_NAME} PRIVATE
#   ${TINYUSB_SRC}
#   ${LIB_SOURCE_DIR}/usb
#   ${LIB_SOURCE_DIR}/usb/midi_usb_bridge
#   ${ARDUINO_MIDI_LIBRARY_SRC}
#   ${CMAKE_CURRENT_LIST_DIR}/../../modules/Arduino-USBMIDI/src
# )

include(tinyusb.cmake)

set(MCUX_SDK_PROJECT_NAME ${EXECUTABLE_NAME})
include(${CMAKE_CURRENT_LIST_DIR}/../sdk/components/arduino_midi_library/src/component_arduino_midi_library.cmake)





endif(WITH_USB)

