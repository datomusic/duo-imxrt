#!/usr/bin/env bash
set -e

SCRIPT_PATH=$(dirname "$(realpath "$0")")
APP_PATH="$1"
TOOLCHAIN_FILE=${TOOLCHAIN_FILE:-"$SCRIPT_PATH/../sdk/armgcc/arm-none-eabi-gcc.cmake"}

echo "Toolchain: $TOOLCHAIN_FILE"

shift

CMAKE_ARGS=$*

BUILD_TYPE="release"

if [[ -e "$APP_PATH" ]]; then
  pushd "$APP_PATH"
  cmake \
    -DTOOLCHAIN_DIR="$TOOLCHAIN_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=flexspi_nor_$BUILD_TYPE  \
    $CMAKE_ARGS \
    -S . -B build

  pushd build
  make -j 3

  pushd flexspi_nor_$BUILD_TYPE

  echo "Generating .bin"
  arm-none-eabi-objcopy -v -O binary \
    -j.flash_config \
    -j.ivt \
    -j.interrupts \
    -j.text \
    -j.ARM.extab \
    -j.ARM \
    -j.ctors \
    -j.dtors \
    -j.preinit_array \
    -j.init_array \
    -j.fini_array \
    -j.interrupts_ram \
    -j.data \
    -j.ram_function \
    -j.ncache.init \
    -j.ncache\
    -j.bss\
    -j.bss.dma\
    -j.heap\
    -j.stack\
    -j.ARM.attributes\
    *.elf ./duo_firmware.bin
else
  echo "Error: Supply directory containing application to build"
fi
