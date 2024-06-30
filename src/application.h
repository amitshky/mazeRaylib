#pragma once

#include <stdint.h>
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
    void (*Init)(struct Application* const this, const char* path);
    void (*Cleanup)(struct Application* const this);
    void (*OnUpdate)(struct Application* const this);

    void (*LoadMap)(struct Application* const this, const char* path);
    void (*CameraUpdate)(struct Application* const this);

    // variables
    Camera3D camera;
    Player player;
    char* mapLayout; // read from file in Init()
    Wall* walls;
    uint64_t wallsNum; // num of elements in walls
} Application;

void Init(Application* const this, const char* path);
void Cleanup(Application* const this);
void OnUpdate(Application* const this);

void LoadMap(Application* const this, const char* path);
void CameraUpdate(Application* const this);
