set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../src/lib)
set(LIB_INCLUDE_DIRS ${LIB_SOURCE_DIR})

file(GLOB LIB_SRC_FILES
    ${LIB_SOURCE_DIR}/*.cpp
    ${LIB_SOURCE_DIR}/*.c
)
