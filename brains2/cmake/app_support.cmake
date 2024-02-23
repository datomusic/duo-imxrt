set(APPS_DIR ${CMAKE_CURRENT_LIST_DIR}/../apps)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
  ${APPS_DIR}/support
)
target_sources(${EXECUTABLE_NAME} PRIVATE
  ${APPS_DIR}/support/app.cpp
)
