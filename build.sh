#!/bin/bash

# exit immediately if a command exits with a non-zero status
set -e

BUILD_DIR="build"

# check if build/ directory exists and create build/ if it doesn't
if [ ! -d ./build/ ]; then
	mkdir build/
fi

EXEC_FILE=$BUILD_DIR"/main"

SRC_FILES="\
	src/main.c \
"

INCLUDE_DIR="\
	./ext/raylib/src/ \
"

LINK_DIR="\
	./ext/raylib/build/raylib/ \
"

LINK_LIB="\
	-lc \
	-lm \
	-lraylib \
"

echo "Compiling..."

gcc $SRC_FILES -o build/main -I$INCLUDE_DIR -L$LINK_DIR $LINK_LIB -Wall -Wextra -std=c17

echo "Compiled to "$EXEC_FILE 
