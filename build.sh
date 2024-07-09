#!/bin/bash

# exit immediately if a command exits with a non-zero status
set -e

BUILD_DIR="build"

# check if build/ directory exists and create build/ if it doesn't
if [ ! -d "./$BUILD_DIR/" ]; then
    mkdir "$BUILD_DIR"
fi

EXEC_FILE="$BUILD_DIR/main"

SRC_FILES="\
    src/main.c \
    src/application.c \
    src/camera.c \
    src/entities.c \
    src/utils.c \
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
CFLAGS="-std=c17 -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer"

echo "Compiling..."

gcc $SRC_FILES -o $EXEC_FILE $INCLUDE_DIR $LINK_DIR $LINK_LIB $WARNING_FLAGS $CFLAGS

echo "Compiled to "$EXEC_FILE 
