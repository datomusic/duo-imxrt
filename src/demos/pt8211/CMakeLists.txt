CMAKE_MINIMUM_REQUIRED (VERSION 3.10.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(pt8211_demo-rt1011)
set(EXECUTABLE_NAME pt8211_demo-rt1011.elf)


add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/pt8211_demo.cpp
    ${LIB_SRC_FILES}
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${LIB_INCLUDE_DIRS}
    ../../src/lib
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/build_app.cmake)
