#!/usr/bin/env bash

if [[ -e "$1" ]]; then
  SCRIPT_PATH=$(dirname "$(realpath "$0")")

  BUILD_TYPE="release"
  BIN_NAME="$(pwd)/$1"/build/flexspi_nor_$BUILD_TYPE/duo_firmware.bin

  cd  "$SCRIPT_PATH"/../../tools/updater && ./update_firmware.py  "$BIN_NAME"
else
  echo "Error: Supply directory containing application to load"
fi
