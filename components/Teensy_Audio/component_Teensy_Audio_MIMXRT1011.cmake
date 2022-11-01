include_guard(GLOBAL)
message("Teensy Audio component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/AudioStream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/synth_sine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/output_mqs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/memcpy_audio.S
    ${CMAKE_CURRENT_LIST_DIR}/data_waveforms.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

include(driver_common_MIMXRT1011)
include(driver_sai_MIMXRT1011)
include(CMSIS_DSP_Library)
