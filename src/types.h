#pragma once

typedef struct Config {
    int width;
    int height;
    const char* title;

    const char* mapPath;
    float fovy;
} Config;
