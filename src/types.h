#pragma once

#include "raylib.h"

typedef struct Config {
    int width;
    int height;
    const char* title;

    const char* mapPath;
    float fovy;
} Config;

// set state before collision
typedef struct CollisionState {
    Vector3 position;
} CollisionState;

typedef struct Button {
    const char* text;
    Vector2 position;
    Vector2 size;
    int fontSize;
    Color fgColor;
    Color bgColor;
    Color borderColor;
} Button;
