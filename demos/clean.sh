#!/usr/bin/env sh
set -e

if [[ -e "$1" ]]; then
  pushd ./"$1"
  rm -r ./build
  popd
else
  echo "Error: Supply directory containing demo to clean"
fi
