#!/usr/bin/env bash
set -e

if [[ -e "$1" ]]; then
  TOOLCHAIN_DIR="${TOOLCHAIN_DIR}"
  
  pushd ./"$1"
  cmake -DTOOLCHAIN_DIR="$TOOLCHAIN_DIR" -DCMAKE_TOOLCHAIN_FILE="../../armgcc/arm-none-eabi-gcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=flexspi_nor_debug  -S . -B build
  pushd build
  make -j
  popd
else
  echo "Error: Supply directory containing demo to build"
fi
