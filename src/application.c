#include "application.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rcamera.h"
#include "utils.h"

void Init(Application* const this, const char* path) {
    this->camera = (Camera3D){
        .position = { 75.0f, 250.0f, 75.0f },
        .target = { 0.0f, 0.0f, 0.0f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    this->player = (Player){
        .position = { 0.0f, 0.0f, 0.0f },
        .size = { 8.0f, 8.0f, 8.0f },
        .color = RED,
        .box = {
            (Vector3){ 0.0f, 0.0f, 0.0f },
            (Vector3){ 0.0f, 0.0f, 0.0f }
        },
    };

    this->mapLayout = NULL;
    this->walls = NULL;
    this->wallsNum = 0;
    this->LoadMap(this, path);
}

void Cleanup(Application* const this) {
    free(this->mapLayout);
    this->mapLayout = NULL;
    free(this->walls);
    this->walls = NULL;
    this->wallsNum = 0;
}

void MovePlayer(Player* const player) {
    const float speed = 10.0f * GetFrameTime();
    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= speed;
    } else if (IsKeyDown(KEY_RIGHT)) {
        player->position.x += speed;
    }

    if (IsKeyDown(KEY_UP)) {
        player->position.z -= speed;
    } else if (IsKeyDown(KEY_DOWN)) {
        player->position.z += speed;
    }

    const float padding = 1.0f;
    player->box = (BoundingBox){
        (Vector3) {
            player->position.x - player->size.x / 2.0f - padding, 
            player->position.y - player->size.y / 2.0f - padding, 
            player->position.z - player->size.z / 2.0f - padding 
        },
        (Vector3) {
            player->position.x + player->size.x / 2.0f + padding, 
            player->position.y + player->size.y / 2.0f + padding, 
            player->position.z + player->size.z / 2.0f + padding 
        }
    };
}

void DrawMaze(Application* const app, bool* const collision) {
    if (app->mapLayout == NULL) {
        printf("Map has not been loaded!\n");
        return;
    }

    for (uint64_t i = 0; i < app->wallsNum; ++i) {
        DrawCubeV(app->walls[i].position, app->walls[i].size, app->walls[i].color);

        if (CheckCollisionBoxes(app->walls[i].box, app->player.box) && !(*collision)) {
            (*collision) = true;
        }
    }
}

void OnUpdate(Application* const this) {
    this->CameraUpdate(this);

    MovePlayer(&this->player);
    DrawCubeV(this->player.position, this->player.size, this->player.color);

    bool collision = false;
    DrawMaze(this, &collision);

    if (collision) {
        this->player.color = BLUE;
        collision = false;
    } else {
        this->player.color = RED;
    }
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
    this->walls = (Wall*)malloc(fileSize * sizeof(Wall));

    // to store wall in a serially
    uint64_t wallIdx = 0;
    // for player position
    float x = 0.0f;
    float z = 0.0f;
    for (uint64_t i = 0; i < fileSize; ++i) {
        int ch = fgetc(fp);
        if (ch == EOF)
            break;

        this->mapLayout[i] = (char)ch;

        if ((char)ch == '#') {
            this->walls[wallIdx] = (Wall){
                .position  = { x, 0.0f, z },
                .size = { 10.0f, 10.0f, 10.0f },
                .color  = { 0xbd, 0x78, 0xc9, 0xff },
            };
            this->walls[wallIdx].box = (BoundingBox) {
                (Vector3) {
                    this->walls[wallIdx].position.x - this->walls[wallIdx].size.x / 2.0f, 
                    this->walls[wallIdx].position.y - this->walls[wallIdx].size.y / 2.0f, 
                    this->walls[wallIdx].position.z - this->walls[wallIdx].size.z / 2.0f 
                },
                (Vector3) {
                    this->walls[wallIdx].position.x + this->walls[wallIdx].size.x / 2.0f, 
                    this->walls[wallIdx].position.y + this->walls[wallIdx].size.y / 2.0f, 
                    this->walls[wallIdx].position.z + this->walls[wallIdx].size.z / 2.0f 
                }
            };

            x += 10.0f;
            ++wallIdx;
        } else if ((char)ch == 'x') {
            this->player.position.x = x;
            this->player.position.z = z;
            x += 10.0f;
        } else if ((char)ch == '\n') {
            x = 0.0f;
            z += 10.f;
        } else {
            x += 10.0f;
        }
    }
    this->mapLayout[fileSize] = '\0';
    this->wallsNum = wallIdx;

    if (fclose(fp) == EOF) {
        printf("Error closing file: %s\n", path);
        exit(-1);
    }
}

void CameraUpdate(Application* const this) {
    // press and hold the left mouse button to move the camera
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        HideCursor();
        DisableCursor();
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        ShowCursor();
        EnableCursor();
    }

    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        return;

    // W, A, S, D, E, Q to move the camera's position
    const float distance = 90.0f * GetFrameTime();

    if (IsKeyDown(KEY_A)) { // left
        CameraMoveRight(&this->camera, -distance, true);
    } else if (IsKeyDown(KEY_D)) { // right
        CameraMoveRight(&this->camera, distance, true);
    }

    if (IsKeyDown(KEY_W)) { // front
        CameraMoveForward(&this->camera, distance, true);
    } else if (IsKeyDown(KEY_S)) { // back
        CameraMoveForward(&this->camera, -distance, true);
    }

    if (IsKeyDown(KEY_E)) { // up
        CameraMoveUp(&this->camera, distance);
    } else if (IsKeyDown(KEY_Q)) { // back
        CameraMoveUp(&this->camera, -distance);
    }

    // move the mouse to look up/down/left/right
    const Vector2 delta = GetMouseDelta();
    const float sensitivity = 0.01f * GetFrameTime();
    CameraPitch(&this->camera, -delta.y * sensitivity, true, false, false);
    CameraYaw(&this->camera, -delta.x * sensitivity, false);
}
