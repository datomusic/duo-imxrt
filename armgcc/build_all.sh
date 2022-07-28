#!/bin/sh
./clean.sh
build.sh debug
make -j

./clean.sh
build.sh release
make -j

./clean.sh
build.sh flexspi_nor_debug
make -j

./clean.sh
./build.sh flexspi_nor_release
make -j


