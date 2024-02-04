set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${LIBRARIES_DIR}/Arduino
  ${LIBRARIES_DIR}/Teensy_Audio
)

include(component_Arduino_MIMXRT1011)
include(component_Teensy_Audio_MIMXRT1011)
