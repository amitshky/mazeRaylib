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
    void (*Init)(struct Application* const this, const Config* const config);
    void (*Cleanup)(struct Application* const this);
    void (*Update)(struct Application* const this);
    void (*UpdateOverlay)(struct Application* const this);

    void (*LoadMap)(struct Application* const this, const Config* const config);
    void (*ControlCamera)(Camera3D* const camera);
    void (*ToggleActiveCamera)(struct Application* this);

    // variables
    Camera3D* camera; // active camera
    ActiveCamera activeCamera;
    Camera3D sceneCamera;

    // initialized in `LoadMap` function
    char* mapLayout;
    Player player;
    Entity* entities; // list of walls and enemies
    uint64_t numEntities;
    uint64_t numWalls;
    uint64_t numEnemies;
} Application;

void Init(Application* const this, const Config* const config);
void Cleanup(Application* const this);
void Update(Application* const this);
void UpdateOverlay(Application* const this);

void LoadMap(Application* const this, const Config* const config);
void ControlCamera(Camera3D* const camera);
void ToggleActiveCamera(Application* this);

#define CREATE_APPLICATION() (Application) { \
    .Init = Init, \
    .Cleanup = Cleanup, \
    .Update = Update, \
    .UpdateOverlay = UpdateOverlay, \
    .LoadMap = LoadMap, \
    .ControlCamera = ControlCamera, \
    .ToggleActiveCamera = ToggleActiveCamera, \
}

