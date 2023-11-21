#!/usr/bin/env bash
# set -e

SCRIPT_PATH=$(dirname "$(realpath $0)")
APP_PATH="$1"
TOOLCHAIN_FILE=${TOOLCHAIN_FILE:-"$SCRIPT_PATH/../sdk/armgcc/arm-none-eabi-gcc.cmake"}

echo $TOOLCHAIN_FILE

shift

CMAKE_ARGS=$*

BUILD_TYPE="release"

cmake \
  -DTOOLCHAIN_DIR="$TOOLCHAIN_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=flexspi_nor_$BUILD_TYPE  \
  $CMAKE_ARGS \
  -S . -B build

pushd build
make
