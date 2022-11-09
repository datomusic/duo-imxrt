#!/usr/bin/env bash
set -e

SCRIPT_PATH=$(dirname $(realpath -s $0))/$1
if [[ -e "$SCRIPT_PATH" ]]; then
  pushd "$SCRIPT_PATH"
  rm -r ./build
  popd
else
  echo "Error: Supply directory containing demo to clean"
fi
