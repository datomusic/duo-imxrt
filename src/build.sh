#!/usr/bin/env bash
set -e

SCRIPT_PATH=$(dirname "$(realpath -s $0)")
APP_PATH="$SCRIPT_PATH/$1"

shift

CMAKE_ARGS=$*

if [[ -e "$APP_PATH" ]]; then
  TOOLCHAIN_FILE="$(pwd)/../armgcc/arm-none-eabi-gcc.cmake"
  pushd "$APP_PATH"
  cmake -DTOOLCHAIN_DIR="$TOOLCHAIN_DIR" -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=flexspi_nor_release  $CMAKE_ARGS -S . -B build
  pushd build
  make -j
  popd
else
  echo "Error: Supply directory containing application to build"
fi
