#!/usr/bin/env bash

cmake -Bbuild -H.
pushd build && make && make test
