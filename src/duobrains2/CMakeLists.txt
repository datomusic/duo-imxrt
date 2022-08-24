CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duobrains2)

ENABLE_LANGUAGE(ASM)


set(EXECUTABLE_NAME duobrains2.elf)
set(MCUX_SDK_PROJECT_NAME ${EXECUTABLE_NAME})

SET(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)
SET(ProjDirPath ${ROOT_DIR}/armgcc)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${ROOT_DIR}/src/lib
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    -Wl,--start-group
        m c gcc nosys
    -Wl,--end-group
)

include(${ROOT_DIR}/armgcc/flags.cmake)
include(${ROOT_DIR}/armgcc/config.cmake)
include(${ROOT_DIR}/armgcc/modules.cmake)
