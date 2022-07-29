include_guard(GLOBAL)
message("driver_FastLED component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/FastLED.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)

# FastLED does not include Arduino.h without this define.
add_definitions(-DARDUINO)

# Silence warning about C++17 ignoring the `register` keyword.
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-register")

include(driver_common_MIMXRT1011)
include(component_Arduino_MIMXRT1011)