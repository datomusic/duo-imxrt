# # CROSS COMPILER SETTING
# SET(CMAKE_SYSTEM_NAME Generic)
# CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)

# # THE VERSION NUMBER
# SET (Tutorial_VERSION_MAJOR 1)
# SET (Tutorial_VERSION_MINOR 0)

# # ENABLE ASM
# ENABLE_LANGUAGE(ASM)

# SET(CMAKE_STATIC_LIBRARY_PREFIX)
# SET(CMAKE_STATIC_LIBRARY_SUFFIX)

# SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
# SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

# # CURRENT DIRECTORY
# SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

# # SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
# SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})

# set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# project(arduino-rt1011)

# # set(MCUX_SDK_PROJECT_NAME arduino-rt1011.elf)

# include(${ProjDirPath}/../cmake/flags.cmake)
# include(${ProjDirPath}/../cmake/config.cmake)
# include(${ProjDirPath}/../cmake/build_app.cmake)

# SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/)

# file(GLOB SRC_FILES
#     ${SOURCE_DIR}/main/arduino-rt1011.cpp
# )

# add_executable(${MCUX_SDK_PROJECT_NAME} 
#     ${SRC_FILES}
# )

# add_definitions(-DARDUINO=100)
# add_definitions(-D__arm__)
# add_definitions(-DTEENSYDUINO)
# add_definitions(-D__IMXRT1011__)

# target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
#     ${SOURCE_DIR}/lib
#     ${SOURCE_DIR}/lib/custom_teensy_audio
#     ${ProjDirPath}/../core/Arduino
# )



# include(driver_pwm_MIMXRT1011)
CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(arduino-rt1011)
set(EXECUTABLE_NAME arduino-rt1011.elf)

SET(COMMON_CMAKE ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

add_executable(${EXECUTABLE_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/main/arduino-rt1011.cpp
    ${LIB_SRC_FILES}
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_DIRS}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/build_app.cmake)
