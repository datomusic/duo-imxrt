#!/usr/bin/env bash
set -e

SCRIPT_PATH=$(dirname "$(realpath "$0")")
TOOLCHAIN_FILE=${TOOLCHAIN_FILE:-"$SCRIPT_PATH/../sdk/armgcc/arm-none-eabi-gcc.cmake"}

echo "Toolchain: $TOOLCHAIN_FILE"

cmake \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=flexspi_nor_release  \
  -S . \
  -B build \
  "$@"

cd build && make
