BUILD_TYPE="release"

if [[ -e "$1" ]]; then
  BIN_NAME="$(pwd)/$1"/build/flexspi_nor_$BUILD_TYPE/duo_firmware.bin
  pushd ../updater
  update_firmware.py  "$BIN_NAME"
else
  echo "Error: Supply directory containing application to load"
fi
