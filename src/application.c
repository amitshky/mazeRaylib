#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "rcamera.h"
#include "raymath.h"

#include "camera.h"
#include "utils.h"


void Init(Application* const this, const Config* const config) {
    this->sceneCamera = (Camera3D) {
        .position = { 75.0f, 200.0f, 76.5f },
        .target   = { 75.0f,   0.0f, 75.5f },
        .up       = {  0.0f,   1.0f,  0.0f },
        .fovy     = config->fovy,
        .projection = CAMERA_PERSPECTIVE,
    };

    this->mapLayout = NULL;
    this->entities = NULL;
    this->numEntities = 0;
    this->numWalls = 0;
    this->numEnemies = 0;

    this->LoadMap(this, config);

    this->camera = &this->player.camera;
    this->activeCamera = PLAYER_CAMERA;
    HideCursor();
    DisableCursor();
}

void Cleanup(Application* const this) {
    free(this->mapLayout);
    this->mapLayout = NULL;

    free(this->entities);
    this->entities = NULL;
    this->numEntities = 0;
    this->numWalls = 0;
    this->numEnemies = 0;
}

void Update(Application* const this) {
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

    // set state before collision
    CollisionState state = {
        .position = this->player.position,
    };

    switch (this->activeCamera) {
        case PLAYER_CAMERA:
            MovePlayer(&this->player);
            break;

        case SCENE_CAMERA:
            this->ControlCamera(this->camera);
            break;
    }

    // press left mouse button to shoot
    if (this->activeCamera == PLAYER_CAMERA && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (uint64_t i = 0; i < this->numEntities; ++i) {
            Ray ray = {
                .position = this->player.position,
                .direction = GetCameraForward(&this->player.camera),
            };

            RayCollision hitInfo = GetRayCollisionBox(ray, this->entities[i].hitbox);

            if (hitInfo.hit) {
                if (this->entities[i].type == ENTITY_WALL)
                    break;
                else if (this->entities[i].type == ENTITY_ENEMY) {
                    this->entities[i].enemy.health -= this->player.damageVal;
                    break;
                }
            }
        }
    }

    for (uint64_t i = 0; i < this->numEntities; ++i) {
        if (CheckCollisionBoxes(this->player.hitbox, this->entities[i].hitbox)) {
            // TODO: fix camera turn on side collision
            PlayerOnCollision(&this->player, &state);
        }

        // remove the dead enemies
        if (this->entities[i].type == ENTITY_ENEMY && this->entities[i].enemy.health <= 0.0f) {
            for (uint64_t j = i; j < this->numEntities; ++j) {
                if (j + 1 < this->numEntities) {
                    this->entities[j] = this->entities[j + 1];
                }
            }
            --this->numEnemies;
            --this->numEntities;
        }

        DrawCubeV(this->entities[i].position, this->entities[i].size, this->entities[i].color);
        if (this->activeCamera == SCENE_CAMERA) {
            DrawBoundingBox(this->entities[i].hitbox, GREEN);
        } else if (this->activeCamera == PLAYER_CAMERA) {
            DrawCubeWiresV(this->entities[i].position, this->entities[i].size, BLUE);
        }

    }

    if (this->activeCamera == SCENE_CAMERA) {
        DrawCubeV(this->player.position, this->player.size, this->player.color);
        DrawBoundingBox(this->player.hitbox, GREEN);
    }
}

void UpdateOverlay(Application* const this) {
    // render number of enemies remaining
    char text[255] = {};
    sprintf(text, "Enemies remaining: %lu", this->numEnemies);
    DrawText(text, 10, 32, 20, WHITE);

    if (this->activeCamera == PLAYER_CAMERA) {
        DrawCrosshair();
    }
}

/**
 * loads map from file
 */
void LoadMap(Application* const this, const Config* const config) {
    FILE* fp = fopen(config->mapPath, "r");
    if (fp == NULL)
    {
        printf("Error opening file: %s\n", config->mapPath);
        exit(-1);
    }

    fseek(fp, 0l, SEEK_END); // seek to the end of file
    const uint64_t fileSize = (const uint64_t)ftell(fp);
    fseek(fp, 0l, 0); // seek to the start of the file

    this->mapLayout = (char*)malloc((fileSize + 1) * sizeof(char));

    // reallocated later
    this->entities = (Entity*)malloc(fileSize * sizeof(Entity));

    // to keep count
    this->numEntities = 0;
    this->numEnemies = 0;
    this->numWalls = 0;
    // for player, enemy, etc position
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
                this->entities[this->numEntities] = InitWall((Vector3) { x, 0.0f, z });
                ++this->numWalls;
                ++this->numEntities;
            } else if ((char)ch == '>' || (char)ch == '<' || (char)ch == 'v' || (char)ch == '^') { // player
                // the characters specify direction the player is facing
                this->player = InitPlayer((Vector3) { x, 0.0f, z }, config->fovy, (char)ch);
            } else if ((char)ch == 'e') { // enemy
                this->entities[this->numEntities] = InitEnemy((Vector3) { x, 0.0f, z }, 10.0f);
                ++this->numEnemies;
                ++this->numEntities;
            }

            x += 10.0f;
        }
    }
    this->mapLayout[fileSize] = '\0';

    size_t bytes = this->numEntities * sizeof(Entity);
    Entity* temp = (Entity*)malloc(bytes);
    memcpy(temp, this->entities, bytes);
    free(this->entities);
    this->entities = temp;
    temp = NULL;

    if (fclose(fp) == EOF) {
        printf("Error closing file: %s\n", config->mapPath);
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
