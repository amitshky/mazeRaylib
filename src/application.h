#pragma once

#include "raylib.h"

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox box;
} Player;

typedef struct Wall {
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox box;
} Wall;

typedef struct Application {
    // methods
    void (*Init)(struct Application* const self, const char* path);
    void (*Cleanup)(struct Application* const self);
    void (*OnUpdate)(struct Application* const self);
    void (*CameraUpdate)(struct Application* const self);

    // variables
    Camera3D camera;
    Player player;
    char* mapLayout; // read from file in Init()
} Application;

void Init(Application* const self, const char* path);
void Cleanup(Application* const self);
void OnUpdate(Application* const self);
void CameraUpdate(Application* const self);
