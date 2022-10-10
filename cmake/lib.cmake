set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../src/lib)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${LIB_SOURCE_DIR}
)

file(GLOB LIB_SRC_FILES
    ${LIB_SOURCE_DIR}/*.cpp
    ${LIB_SOURCE_DIR}/*.c
)

target_sources(${EXECUTABLE_NAME} PRIVATE
  ${LIB_SRC_FILES}
)
