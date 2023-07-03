CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
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

add_executable(tests 
  ${CMAKE_CURRENT_LIST_DIR}/runner.cpp
)
target_link_libraries(tests PRIVATE unity_framework firmware)

add_test(
  NAME tests
  COMMAND $<TARGET_FILE:tests>)
