#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "entities.h"

typedef enum ActiveCamera {
    PLAYER_CAMERA,
    SCENE_CAMERA,
} ActiveCamera;

typedef struct Application {
    // methods
    void (*Init)(struct Application* const this, Config* const config);
    void (*Cleanup)(struct Application* const this);
    void (*OnUpdate)(struct Application* const this);
    void (*UpdateOverlay)(struct Application* const this);

    void (*LoadMap)(struct Application* const this, const char* path);
    void (*ControlCamera)(Camera3D* const camera);

    // variables
    Camera3D* camera; // active camera
    ActiveCamera activeCamera;
    Camera3D sceneCamera;
    Player player;

    Enemy* enemies;
    uint64_t enemiesNum;

    // initialized in `LoadMap` function
    char* mapLayout;
    Wall* walls; // list of all walls
    uint64_t wallsNum; // number of walls
} Application;

void Init(Application* const this, Config* const config);
void Cleanup(Application* const this);
void OnUpdate(Application* const this);
void UpdateOverlay(Application* const this);

void LoadMap(Application* const this, const char* path);
void ControlCamera(Camera3D* const camera);

#define CREATE_APPLICATION() (Application) { \
    .Init = Init, \
    .Cleanup = Cleanup, \
    .OnUpdate = OnUpdate, \
    .LoadMap = LoadMap, \
    .ControlCamera = ControlCamera, \
    .UpdateOverlay = UpdateOverlay, \
}

