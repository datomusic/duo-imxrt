#!/usr/bin/env bash
set -e

SCRIPT_PATH=$(dirname $(realpath -s $0))
APP_PATH="$SCRIPT_PATH/$1"

if [[ -e "$APP_PATH" ]]; then
  TOOLCHAIN_DIR="${TOOLCHAIN_DIR}"
  TOOLCHAIN_FILE="$(pwd)/../armgcc/arm-none-eabi-gcc.cmake"
  pushd "$APP_PATH"
  
  pushd "$APP_PATH"
  cmake -DTOOLCHAIN_DIR="$TOOLCHAIN_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=flexspi_nor_debug  -S . -B build
  pushd build
  make -j
  popd
else
  echo "Error: Supply directory containing application to build"
fi