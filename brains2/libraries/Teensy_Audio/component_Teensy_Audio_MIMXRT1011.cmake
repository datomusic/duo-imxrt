include_guard(GLOBAL)
message("Teensy Audio component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/synth_sine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/memcpy_audio.S
    ${CMAKE_CURRENT_LIST_DIR}/data_waveforms.c
    ${CMAKE_CURRENT_LIST_DIR}/analyze_peak.cpp

    ${CMAKE_CURRENT_LIST_DIR}/effect_bitcrusher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/effect_delay.cpp
    ${CMAKE_CURRENT_LIST_DIR}/effect_multiply.cpp
    ${CMAKE_CURRENT_LIST_DIR}/effect_fade.cpp
    ${CMAKE_CURRENT_LIST_DIR}/filter_variable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/mixer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synth_waveform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synth_dc.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synth_whitenoise.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synth_pinknoise.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synth_simple_drum.cpp
    ${CMAKE_CURRENT_LIST_DIR}/data_bandlimit_step.c
    ${CMAKE_CURRENT_LIST_DIR}/play_memory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/data_ulaw.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_common_MIMXRT1011)
include(driver_sai_MIMXRT1011)
include(CMSIS_DSP_Library)
