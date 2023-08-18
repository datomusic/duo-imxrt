execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND git diff --stat
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_DIFF_OUTPUT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if ("${GIT_DIFF_OUTPUT}" STREQUAL "")
  SET(GIT_DIRTY_STATE "clean")
else()
  SET(GIT_DIRTY_STATE "dirty")
endif()


# Find current tag. If the current commit is not tagged,
# this command will return "undefined".
execute_process(
  COMMAND git name-rev --name-only --tags HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_TAG
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if ("${GIT_TAG}" STREQUAL "undefined")
  SET(GIT_TAG "Development build")
endif()

add_definitions("-DGIT_TAG=\"${GIT_TAG}\"")
add_definitions("-DGIT_HASH=\"${GIT_HASH}\"")
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH} [${GIT_DIRTY_STATE}]\"")
