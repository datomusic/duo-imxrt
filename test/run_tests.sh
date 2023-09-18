#!/usr/bin/env bash

cmake -Bbuild -H.
pushd build && make && ctest --output-on-failure
