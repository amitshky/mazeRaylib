#include "application.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rcamera.h"
#include "utils.h"

void Init(Application* const self, const char* path) {
    self->playerCamera = (Camera3D){
        .position = { 0.0f, 0.0f, 0.0f },
        .target = { 0.0f, 0.0f, 0.0f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    FILE* fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Error opening file: %s\n", path);
        exit(-1);
    }

    fseek(fp, 0l, SEEK_END); // seek to the end of file
    const uint64_t fileSize = ftell(fp);
    fseek(fp, 0l, 0); // seek to the start of the file

    self->mapLayout = (char*)malloc(fileSize + 1);

    int ch;
    int i = 0;

    // for player position
    float x = 0.0f;
    float z = 0.0f;

    while ((ch = fgetc(fp)) != EOF) {
        self->mapLayout[i++] = (char)ch;
        if (ch == 'x') {
            self->playerCamera.position.x = x;
            self->playerCamera.position.z = z;
        }

        x += 10.0f;
        if (ch == '\n') {
            x = 10.0f;
            z += 10.f;
        }
    }
    self->mapLayout[fileSize] = '\0';

    if (fclose(fp) == EOF) {
        printf("Error closing file: %s\n", path);
        exit(-1);
    }
}

void Cleanup(Application* const self) {
    free(self->mapLayout);
}

void OnUpdate(Application* const self) {
    self->UpdatePlayerCamera(self);
    float x = 0.0f;
    float z = 0.0f;

    Vector3 wallPos  = { 0.0f, 0.0f, 0.0f };
    Vector3 wallSize = { 10.0f, 10.0f, 10.0f };
    Color wallColor  = { 193, 116, 93, 255 };

    for (uint64_t i = 0; i < strlen(self->mapLayout); ++i) {
        if (self->mapLayout[i] == '#') {
            wallPos.x = x;
            wallPos.z = z;
            DrawCubeV(wallPos, wallSize, wallColor);
            x += 10.0f;
        } else if (self->mapLayout[i] == '\n') {
            x = 0.0f;
            z += 10.0f;
        } else if (self->mapLayout[i] == '.' || self->mapLayout[i] == 'x') {
            x += 10.0f;
        }
    }
}

void UpdatePlayerCamera(Application* const self) {
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
        CameraMoveRight(&self->playerCamera, -distance, true);
    } else if (IsKeyDown(KEY_D)) { // right
        CameraMoveRight(&self->playerCamera, distance, true);
    }

    if (IsKeyDown(KEY_W)) { // front
        CameraMoveForward(&self->playerCamera, distance, true);
    } else if (IsKeyDown(KEY_S)) { // back
        CameraMoveForward(&self->playerCamera, -distance, true);
    }

    if (IsKeyDown(KEY_E)) { // up
        CameraMoveUp(&self->playerCamera, distance);
    } else if (IsKeyDown(KEY_Q)) { // back
        CameraMoveUp(&self->playerCamera, -distance);
    }

    // move the mouse to look up/down/left/right
    const Vector2 delta = GetMouseDelta();
    const float sensitivity = 0.01f * GetFrameTime();
    CameraPitch(&self->playerCamera, -delta.y * sensitivity, true, false, false);
    CameraYaw(&self->playerCamera, -delta.x * sensitivity, false);
}
