CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
project(sample_player)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME sample_player.elf)

add_executable(${EXECUTABLE_NAME}
    main.cpp
    output.cpp
    AudioSampleSnare.cpp
    AudioSampleKick.cpp
    AudioSampleHihat.cpp
    AudioSampleGong.cpp
    AudioSampleTomtom.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)
