CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

add_library(unity_framework
  ${CMAKE_CURRENT_LIST_DIR}/unity/unity.c
)

add_library(firmware
  ${CMAKE_CURRENT_LIST_DIR}/../firmware/seq.cpp
)

add_executable(tests 
  ${CMAKE_CURRENT_LIST_DIR}/runner.cpp
)
target_link_libraries(tests PRIVATE unity_framework firmware)
target_include_directories(tests PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../
  ${CMAKE_CURRENT_LIST_DIR}/unity
)

add_test(
  NAME tests
  COMMAND $<TARGET_FILE:tests>)
