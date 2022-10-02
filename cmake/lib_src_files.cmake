set(TINYUSB_SRC ${CMAKE_CURRENT_LIST_DIR}/../libraries/tinyusb/src)

set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../src/lib)
set(LIB_INCLUDE_DIRS 
    ${LIB_SOURCE_DIR}
    ${LIB_SOURCE_DIR}/usb
    ${TINYUSB_SRC}
)

file(GLOB LIB_SRC_FILES
    ${LIB_SOURCE_DIR}/*.cpp
    ${LIB_SOURCE_DIR}/usb/usb.cpp
    ${LIB_SOURCE_DIR}/usb/usb_descriptors.c
    ${LIB_SOURCE_DIR}/*.c
)
