#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "raylib.h"

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox hitbox;
} Player;

typedef struct Wall {
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox hitbox;
} Wall;

BoundingBox CreateHitbox(const Vector3 position, const Vector3 size, const Vector3 padding);

typedef struct Application {
    // methods
    void (*Init)(struct Application* const this, const char* path);
    void (*Cleanup)(struct Application* const this);
    void (*OnUpdate)(struct Application* const this);

    void (*LoadMap)(struct Application* const this, const char* path);
    void (*ControlCamera)(struct Application* const this);

    // variables
    Camera3D camera;
    Player player;

    // initialized in `LoadMap` function
    char* mapLayout;
    Wall* walls; // list of all walls
    uint64_t wallsNum; // number of walls
} Application;

void Init(Application* const this, const char* path);
void Cleanup(Application* const this);
void OnUpdate(Application* const this);

void LoadMap(Application* const this, const char* path);
void ControlCamera(Application* const this);

#define CREATE_APPLICATION() (Application) { \
    .Init = Init, \
    .Cleanup = Cleanup, \
    .OnUpdate = OnUpdate, \
    .LoadMap = LoadMap, \
    .ControlCamera = ControlCamera, \
}

