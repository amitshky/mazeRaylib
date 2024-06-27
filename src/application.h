#pragma once

#include "raylib.h"

typedef struct Application {
    // public methods
    void (*Init)(struct Application* const self, const char* path);
    void (*Cleanup)(struct Application* const self);

    void (*OnUpdate)(struct Application* const self);

    // private variables
    char* mapLayout; // read from file in Init()
} Application;

void Init(Application* const self, const char* path);
void Cleanup(Application* const self);
void OnUpdate(Application* const self);
