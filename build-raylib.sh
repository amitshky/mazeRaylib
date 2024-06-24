#!/bin/bash

set -e

cd ext/raylib/
cmake -S . -B build
cmake --build build
