#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "rcamera.h"
#include "raymath.h"

#include "camera.h"
#include "utils.h"


void Init(Application* const this, Config* const config) {
    this->sceneCamera = (Camera3D) {
        .position = { 75.0f, 200.0f, 76.5f },
        .target   = { 75.0f,   0.0f, 75.5f },
        .up       = {  0.0f,   1.0f,  0.0f },
        .fovy     = config->fovy,
        .projection = CAMERA_PERSPECTIVE,
    };

    this->player = (Player) {
        .size = { 4.0f, 10.0f, 4.0f },
        .color = GREEN,
        .speed = 15.0f,
        .hitboxPadding = { 0.5f, 0.0f, 0.5f },
    };
    this->player.camera = (Camera3D) {
        .position = { 0.0f, 0.0f, 0.0f },
        .target   = { 0.0f, 0.0f, 0.0f },
        .up       = { 0.0f, 1.0f, 0.0f },
        .fovy     = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };
    this->player.hitbox = CreateHitbox(this->player.camera.position, this->player.size, this->player.hitboxPadding);

    this->camera = &this->player.camera;
    this->activeCamera = PLAYER_CAMERA;
    HideCursor();
    DisableCursor();

    this->mapLayout = NULL;
    this->walls = NULL;
    this->wallsNum = 0;
    this->enemies = NULL;
    this->enemiesNum = 0;
    this->LoadMap(this, config->mapPath);
}

void Cleanup(Application* const this) {
    free(this->mapLayout);
    this->mapLayout = NULL;

    free(this->walls);
    this->walls = NULL;
    this->wallsNum = 0;

    free(this->enemies);
    this->enemies = NULL;
    this->enemiesNum = 0;
}

void OnUpdate(Application* const this) {
    // switch camera on `tab` key press
    if (IsKeyPressed(KEY_TAB)) {
        switch (this->activeCamera) {
            case PLAYER_CAMERA: 
                this->camera = &this->sceneCamera;
                this->activeCamera = SCENE_CAMERA;
                ShowCursor();
                EnableCursor();
                break;

            case SCENE_CAMERA:
                this->camera = &this->player.camera;
                this->activeCamera = PLAYER_CAMERA;
                HideCursor();
                DisableCursor();
                break;
        }
    }

    Vector3 prevPlayerPosition = this->player.camera.position;
    switch (this->activeCamera) {
        case PLAYER_CAMERA:
            MovePlayer(&this->player);
            break;

        case SCENE_CAMERA:
            this->ControlCamera(this->camera);
            break;
    }

    for (uint64_t i = 0; i < this->wallsNum; ++i) {
        DrawCubeV(this->walls[i].position, this->walls[i].size, this->walls[i].color);
        DrawBoundingBox(this->walls[i].hitbox, BLUE);

        if (CheckCollisionBoxes(this->player.hitbox, this->walls[i].hitbox)) {
            this->player.camera.position = prevPlayerPosition;
        }
    }

    for (uint64_t i = 0; i < this->enemiesNum; ++i) {
        DrawCubeV(this->enemies[i].position, this->enemies[i].size, this->enemies[i].color);
        DrawBoundingBox(this->enemies[i].hitbox, BLUE);

        if (CheckCollisionBoxes(this->player.hitbox, this->enemies[i].hitbox)) {
            this->player.camera.position = prevPlayerPosition;
        }

        // TODO: cast ray from the center to shoot
    }

    // DrawCubeV(this->player.camera.position, this->player.size, this->player.color);
    DrawBoundingBox(this->player.hitbox, GREEN);

    // TODO: draw cross hair
}

/**
 * loads map from file
 */
void LoadMap(Application* const this, const char* path) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Error opening file: %s\n", path);
        exit(-1);
    }

    fseek(fp, 0l, SEEK_END); // seek to the end of file
    const uint64_t fileSize = (const uint64_t)ftell(fp);
    fseek(fp, 0l, 0); // seek to the start of the file

    this->mapLayout = (char*)malloc((fileSize + 1) * sizeof(char));

    // these are reallocated later
    this->walls = (Wall*)malloc(fileSize * sizeof(Wall));
    this->enemies = (Enemy*)malloc(fileSize * sizeof(Enemy));

    // to keep count
    size_t wallIdx = 0;
    size_t enemyIdx = 0;
    // for player position
    float x = 0.0f;
    float z = 0.0f;
    for (uint64_t i = 0; i < fileSize; ++i) {
        int ch = fgetc(fp);
        if (ch == EOF)
            break;

        this->mapLayout[i] = (char)ch;

        if ((char)ch == '\n') {
            x = 0.0f;
            z += 10.f;
        } else {
            if ((char)ch == '#') { // wall
                this->walls[wallIdx] = (Wall) {
                    .position  = { x, 0.0f, z },
                    .size = { 10.0f, 10.0f, 10.0f },
                    .color  = { 0xbd, 0x78, 0xc9, 0xff },
                };
                this->walls[wallIdx].hitbox = 
                    CreateHitbox(this->walls[wallIdx].position, this->walls[wallIdx].size, Vector3Zero());

                ++wallIdx;
            } else if ((char)ch == 'x') { // player
                this->player.camera.position.x = x;
                this->player.camera.position.z = z;
            } else if ((char)ch == 'e') { // enemy
                // TODO: reduce the size of the enemies and change position.y so that the enemies touch the ground
                this->enemies[enemyIdx] = (Enemy) {
                    .position = { x, 0.0f, z },
                    .size = { 8.0f, 10.0f, 8.0f },
                    .color = RED,
                };
                this->enemies[enemyIdx].hitbox = 
                    CreateHitbox(this->enemies[enemyIdx].position, this->enemies[enemyIdx].size, Vector3Zero());

                ++enemyIdx;
            }

            x += 10.0f;
        }
    }
    this->mapLayout[fileSize] = '\0';
    this->wallsNum = wallIdx;
    this->enemiesNum = enemyIdx;

    this->player.camera.target = Vector3Add(this->player.camera.position, (Vector3) { 1.0f, 0.0f, 0.0f });
    this->player.hitbox = CreateHitbox(this->player.camera.position, this->player.size, this->player.hitboxPadding);

    void* temp = malloc(this->wallsNum * sizeof(Wall));
    memcpy(temp, this->walls, this->wallsNum * sizeof(Wall));
    free(this->walls);
    this->walls = (Wall*)temp;
    temp = NULL;

    temp = malloc(this->enemiesNum * sizeof(Enemy));
    memcpy(temp, this->enemies, this->enemiesNum * sizeof(Enemy));
    free(this->enemies);
    this->enemies = (Enemy*)temp;
    temp = NULL;

    if (fclose(fp) == EOF) {
        printf("Error closing file: %s\n", path);
        exit(-1);
    }
}

void ControlCamera(Camera3D* const camera) {
    // press and hold the right mouse button or middle to move the camera
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
        HideCursor();
        DisableCursor();
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) || IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
        ShowCursor();
        EnableCursor();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        CameraPan(camera);
    else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        CameraOrbit(camera);
}
