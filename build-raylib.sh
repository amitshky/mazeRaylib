#!/bin/bash

set -e

git submodule update --init --recursive
cd ext/raylib/
cmake -S . -B build
cmake --build build
