#!/bin/bash

# exit immediately if a command exits with a non-zero status
set -e

BUILD_DIR="build/rel"

# check if build/ directory exists and create build/ if it doesn't
if [ ! -d ./build/ ]; then
    mkdir build/
fi

if [ ! -d ./build/rel/ ]; then
    mkdir build/rel/
fi

EXEC_FILE=$BUILD_DIR"/main"

SRC_FILES="\
    src/main.c \
    src/application.c \
    src/camera.c \
"

INCLUDE_DIR="\
    -I ext/raylib/src/ \
    -I src/ \
"

LINK_DIR="\
    -L ext/raylib/build/raylib/ \
"

LINK_LIB="\
    -lc \
    -lm \
    -lraylib \
"

WARNING_FLAGS="-Wall -Wextra -Wconversion -Werror"
CFLAGS="-std=c17 -O3"

echo "Compiling..."

gcc $SRC_FILES -o $EXEC_FILE $INCLUDE_DIR $LINK_DIR $LINK_LIB $WARNING_FLAGS $CFLAGS

echo "Compiled to "$EXEC_FILE 

