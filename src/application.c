#include "application.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rcamera.h"
#include "utils.h"

/**
 * loads map from file, allocates `mapLayout` so dont forget to free it
 */
void LoadMap(char** const mapLayout, const char* path, Player* const player) {
    FILE* fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Error opening file: %s\n", path);
        exit(-1);
    }

    fseek(fp, 0l, SEEK_END); // seek to the end of file
    const uint64_t fileSize = (const uint64_t)ftell(fp);
    fseek(fp, 0l, 0); // seek to the start of the file

    (*mapLayout) = (char*)malloc((fileSize + 1) * sizeof(char));

    int ch;
    int i = 0;
    // for player position
    float x = 0.0f;
    float z = 0.0f;
    while ((ch = fgetc(fp)) != EOF) {
        (*mapLayout)[i++] = (char)ch;

        if (ch == 'x') {
            player->position.x = x;
            player->position.z = z;
        } else if (ch == '\n') {
            x = 10.0f;
            z += 10.f;
        } else {
            x += 10.0f;
        }
    }
    (*mapLayout)[fileSize] = '\0';
    player->box = (BoundingBox){
        (Vector3) {
            player->position.x - player->size.x / 2.0f, 
            player->position.y - player->size.y / 2.0f, 
            player->position.z - player->size.z / 2.0f 
        },
        (Vector3) {
            player->position.x + player->size.x / 2.0f, 
            player->position.y + player->size.y / 2.0f, 
            player->position.z + player->size.z / 2.0f 
        }
    };

    if (fclose(fp) == EOF) {
        printf("Error closing file: %s\n", path);
        exit(-1);
    }
}

void Init(Application* const self, const char* path) {
    self->camera = (Camera3D){
        .position = { 75.0f, 250.0f, 75.0f },
        .target = { 0.0f, 0.0f, 0.0f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    self->player = (Player){
        .position = { 0.0f, 0.0f, 0.0f },
        .size = { 8.0f, 8.0f, 8.0f },
        .color = RED,
        .box = {
            (Vector3){ 0.0f, 0.0f, 0.0f },
            (Vector3){ 0.0f, 0.0f, 0.0f }
        },
    };

    self->mapLayout = NULL;
    LoadMap(&self->mapLayout, path, &self->player);
}

void Cleanup(Application* const self) {
    free(self->mapLayout);
    self->mapLayout = NULL;
}

void MovePlayer(Player* const player) {
    const float speed = 60.0f * GetFrameTime();
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

    player->box = (BoundingBox){
        (Vector3) {
            player->position.x - player->size.x / 2.0f, 
            player->position.y - player->size.y / 2.0f, 
            player->position.z - player->size.z / 2.0f 
        },
        (Vector3) {
            player->position.x + player->size.x / 2.0f, 
            player->position.y + player->size.y / 2.0f, 
            player->position.z + player->size.z / 2.0f 
        }
    };
}

void DrawMaze(char** const mapLayout, Wall* const wall, Player* const player, bool* const collision) {
    if ((*mapLayout) == NULL) {
        printf("Map has not been loaded!\n");
        return;
    }

    float x = 0.0f;
    float z = 0.0f;
    const size_t mapSize = strlen(*mapLayout);
    for (uint64_t i = 0; i < mapSize; ++i) {
        if ((*mapLayout)[i] == '#') {
            wall->position.x = x;
            wall->position.z = z;
            x += 10.0f;
            DrawCubeV(wall->position, wall->size, wall->color);

            wall->box = (BoundingBox){
                (Vector3) {
                    wall->position.x - wall->size.x / 2.0f, 
                    wall->position.y - wall->size.y / 2.0f, 
                    wall->position.z - wall->size.z / 2.0f 
                },
                (Vector3) {
                    wall->position.x + wall->size.x / 2.0f, 
                    wall->position.y + wall->size.y / 2.0f, 
                    wall->position.z + wall->size.z / 2.0f 
                }
            };

            if (CheckCollisionBoxes(wall->box, player->box) && !(*collision)) {
                (*collision) = true;
            }

        } else if ((*mapLayout)[i] == '\n') {
            x = 0.0f;
            z += 10.0f;
        } else if ((*mapLayout)[i] == '.' || (*mapLayout)[i] == 'x') {
            x += 10.0f;
        }
    }
}

void OnUpdate(Application* const self) {
    self->CameraUpdate(self);
    Wall wall = {
        .position  = { 0.0f, 0.0f, 0.0f },
        .size = { 10.0f, 10.0f, 10.0f },
        .color  = { 0xbd, 0x78, 0xc9, 0xff },
        .box = {
            (Vector3){ 0.0f, 0.0f, 0.0f },
            (Vector3){ 0.0f, 0.0f, 0.0f }
        },
    };

    MovePlayer(&self->player);
    DrawCubeV(self->player.position, self->player.size, self->player.color);
    bool collision = false;
    DrawMaze(&self->mapLayout, &wall, &self->player, &collision);
    if (collision) {
        self->player.color = BLUE;
        collision = false;
    } else {
        self->player.color = RED;
    }
}

void CameraUpdate(Application* const self) {
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
        CameraMoveRight(&self->camera, -distance, true);
    } else if (IsKeyDown(KEY_D)) { // right
        CameraMoveRight(&self->camera, distance, true);
    }

    if (IsKeyDown(KEY_W)) { // front
        CameraMoveForward(&self->camera, distance, true);
    } else if (IsKeyDown(KEY_S)) { // back
        CameraMoveForward(&self->camera, -distance, true);
    }

    if (IsKeyDown(KEY_E)) { // up
        CameraMoveUp(&self->camera, distance);
    } else if (IsKeyDown(KEY_Q)) { // back
        CameraMoveUp(&self->camera, -distance);
    }

    // move the mouse to look up/down/left/right
    const Vector2 delta = GetMouseDelta();
    const float sensitivity = 0.01f * GetFrameTime();
    CameraPitch(&self->camera, -delta.y * sensitivity, true, false, false);
    CameraYaw(&self->camera, -delta.x * sensitivity, false);
}
