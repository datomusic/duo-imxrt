#!/usr/bin/env bash
set -e

# Make sure the directory we're cleaning is a child of where this script is located
# to avoid accidents like deleting '/'.
SCRIPT_PATH=$(dirname $(realpath $0))
APP_PATH="$SCRIPT_PATH/$1"

if [[ -e "$APP_PATH" ]]; then
  pushd "$APP_PATH"
  rm -r ./build
  popd
else
  echo "Error: Supply directory containing application to clean"
fi
