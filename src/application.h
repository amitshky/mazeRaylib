#pragma once

#include "raylib.h"

typedef struct Application {
    // public methods
    void (*Init)(struct Application* const);
    void (*Cleanup)(struct Application* const);

    void (*OnUpdate)(struct Application* const);

    // private variables
    const char* mapLayout;
} Application;

// void Init(Application* const self);
// void Cleanup(Application* const self);
void OnUpdate(Application* const self);
