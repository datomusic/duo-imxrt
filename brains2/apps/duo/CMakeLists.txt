CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(duo)

ENABLE_LANGUAGE(ASM)

set(EXECUTABLE_NAME duo-imxrt.elf)

add_executable(${EXECUTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tempo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/adapters/fast_led/FastLED.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/effect_custom_envelope.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../shared/duo/seq.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../../../
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
    ${CMAKE_CURRENT_LIST_DIR}/../libraries/
    ${CMAKE_CURRENT_LIST_DIR}/../../core/adapters/fast_led
)

set(WITH_USB true)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/build_app.cmake)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD}
)

# Persistent storage: littlefs on the tail of the QSPI flash, exposed
# through the musin filesystem port and newlib stdio.
include(driver_flexspi_MIMXRT1011)
include(driver_cache_armv7_m7_MIMXRT1011)

target_sources(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/filesystem_smoke_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings_storage.cpp
    ${LIBRARIES_DIR}/littlefs/lfs.c
    ${LIBRARIES_DIR}/littlefs/lfs_util.c
    ${LIBRARIES_DIR}/musin/settings/settings_manager.cpp
    ${LIBRARIES_DIR}/musin/ports/imxrt/filesystem/filesystem.cpp
    ${LIBRARIES_DIR}/musin/ports/imxrt/filesystem/storage_region.cpp
    ${LIBRARIES_DIR}/musin/ports/imxrt/filesystem/lfs_stdio.c
    ${SDK_DIR}/components/flash/mflash/mimxrt1011/mflash_drv.c
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${LIBRARIES_DIR}
    ${LIBRARIES_DIR}/littlefs
    ${LIBRARIES_DIR}/etl/include
    ${LIBRARIES_DIR}/musin/ports/imxrt/filesystem
    ${SDK_DIR}/components/flash/mflash
    ${SDK_DIR}/components/flash/mflash/mimxrt1011
)

