CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
project(duo-tests)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

add_library(unity_framework
  ${CMAKE_CURRENT_LIST_DIR}/unity/unity.c
)

target_include_directories(unity_framework PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/unity
)

add_library(firmware
  ${CMAKE_CURRENT_LIST_DIR}/../firmware/seq.cpp
)

target_include_directories(firmware PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../
)

add_executable(sequencer_test 
  ${CMAKE_CURRENT_LIST_DIR}/firmware/sequencer_test.cpp
)
target_link_libraries(sequencer_test PRIVATE unity_framework firmware)

add_test(
  NAME sequencer_test
  COMMAND $<TARGET_FILE:sequencer_test>)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -Wall -Wpedantic -Werror" CACHE INTERNAL "c compiler flags release")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wall -Wpedantic -Werror" CACHE INTERNAL "cxx compiler flags release")
