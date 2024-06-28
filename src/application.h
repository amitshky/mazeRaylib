#pragma once

#include "raylib.h"

typedef struct Application {
    // methods
    void (*Init)(struct Application* const self, const char* path);
    void (*Cleanup)(struct Application* const self);
    void (*OnUpdate)(struct Application* const self);
    void (*UpdatePlayerCamera)(struct Application* const self);

    // variables
    char* mapLayout; // read from file in Init()
    Camera3D playerCamera;
} Application;

void Init(Application* const self, const char* path);
void Cleanup(Application* const self);
void OnUpdate(Application* const self);
void UpdatePlayerCamera(Application* const self);
