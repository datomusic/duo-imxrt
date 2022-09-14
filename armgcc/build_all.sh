#!/bin/sh
./clean.sh

build.sh debug
make -j

build.sh release
make -j

build.sh flexspi_nor_debug
make -j

./build.sh flexspi_nor_release
make -j


