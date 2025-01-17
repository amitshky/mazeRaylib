#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "types.h"
#include "entities.h"

// #define MENU_BG_COLOR GetColor(0x475c7fff)
#define MENU_BG_COLOR GetColor(0x82a7aaff)


typedef enum GameState {
    // START,
    GAME,
    PAUSE,
    END,
} GameState;

typedef enum ActiveCamera {
    PLAYER_CAMERA,
#ifdef _DEBUG
    SCENE_CAMERA,
#endif
} ActiveCamera;

typedef struct Application {
    // methods
    void (*Init)(struct Application* const this, const Config config);
    void (*Cleanup)(struct Application* const this);
    void (*Update)(struct Application* const this);
    void (*UpdateOverlay)(struct Application* const this);

    void (*LoadMap)(struct Application* const this);
    void (*ParseMap)(struct Application* const this);

#ifdef _DEBUG
    void (*ControlCamera)(Camera3D* const camera);
    void (*ToggleActiveCamera)(struct Application* const this);
#endif

    void (*UpdateGame)(struct Application* const this); // game loop
    void (*UpdatePauseScreen)(struct Application* const this); // pause/start screen
    void (*UpdateEndScreen)(struct Application* const this); // win/lose screen

    // variables
    GameState gameState;
    Camera3D* camera; // active camera
    ActiveCamera activeCamera;
    Camera3D sceneCamera;

    bool pauseScreenLoaded; // to prevent keypresses from overlapping
    bool isCursorVisibilityToggled; // to be used with `ToggleCursorVisibility()`
    Config config;

    // initialized in `LoadMap` function
    char* mapLayout;
    uint64_t numMapLayout;
    Player player;
    Entity* entities; // list of walls and enemies
    uint64_t numEntities;
    uint64_t numWalls;
    uint64_t numEnemies;
} Application;

void Init(Application* const this, const Config config);
void Cleanup(Application* const this);
void Update(Application* const this);
void UpdateOverlay(Application* const this);

void LoadMap(Application* const this);
void ParseMap(Application* const this);

#ifdef _DEBUG
void ControlCamera(Camera3D* const camera);
void ToggleActiveCamera(Application* const this);
#endif

void UpdateGame(Application* const this);
void UpdatePauseScreen(Application* const this);
void UpdateEndScreen(Application* const this);

void Shoot(struct Application* const this);

// only add these in debug builds
#ifdef _DEBUG
#define CREATE_APPLICATION_DEBUG_EXT \
    .ControlCamera = ControlCamera, \
    .ToggleActiveCamera = ToggleActiveCamera,
#elif defined _RELEASE
#define CREATE_APPLICATION_DEBUG_EXT
#endif

#define CREATE_APPLICATION() (Application) { \
    .Init = Init, \
    .Cleanup = Cleanup, \
    .Update = Update, \
    .UpdateOverlay = UpdateOverlay, \
    .LoadMap = LoadMap, \
    .ParseMap = ParseMap, \
    .UpdateGame = UpdateGame, \
    .UpdatePauseScreen = UpdatePauseScreen, \
    .UpdateEndScreen = UpdateEndScreen, \
    CREATE_APPLICATION_DEBUG_EXT \
}
