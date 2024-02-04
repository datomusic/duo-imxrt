if(NOT DEFINED BOARD)
    set(BOARD "DUO_BRAINS_2.1")
    message(STATUS "Using default BOARD: " ${BOARD})
endif(NOT DEFINED BOARD)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
  DATO_BOARD=\"${BOARD}\")
