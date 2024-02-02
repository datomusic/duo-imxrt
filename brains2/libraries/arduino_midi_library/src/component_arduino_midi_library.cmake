include_guard(GLOBAL)
message("arduino_midi_library component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/midi_Namespace.h
${CMAKE_CURRENT_LIST_DIR}/midi_Defs.h
${CMAKE_CURRENT_LIST_DIR}/midi_Message.h
${CMAKE_CURRENT_LIST_DIR}/midi_Platform.h
${CMAKE_CURRENT_LIST_DIR}/midi_Settings.h
${CMAKE_CURRENT_LIST_DIR}/MIDI.cpp
${CMAKE_CURRENT_LIST_DIR}/MIDI.hpp
${CMAKE_CURRENT_LIST_DIR}/MIDI.h
${CMAKE_CURRENT_LIST_DIR}/serialMIDI.h
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_common_MIMXRT1011)