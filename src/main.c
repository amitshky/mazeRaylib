#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "application.h"
#include "camera.h"

void UIDrawGuides();

int main(void) {
    InitWindow(640, 640, "Maze");
    SetTargetFPS(60);

    Application app = {
        .Init = Init,
        .Cleanup = Cleanup,
        .OnUpdate = OnUpdate,
    };
    app.Init(&app, "assets/map/map1.txt");

    Camera3D playerCamera = {
        .position = app.playerPosition,
        .target = { 0.0f, 0.0f, 0.0f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(playerCamera);

        UIDrawGuides();
        app.OnUpdate(&app);

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
        CameraController(&playerCamera);
    }

    app.Cleanup(&app);
    CloseWindow();
    return 0;
}


void UIDrawGuides() {
    DrawLine3D((Vector3){ 0.0f, -1000.0f, 0.0f }, (Vector3){ 0.0f, 1000.0f, 0.0f }, GREEN);
    DrawLine3D((Vector3){ -1000.0f, 0.0f, 0.0f }, (Vector3){ 1000.0f, 0.0f, 0.0f }, RED);
    DrawLine3D((Vector3){ 0.0f, 0.0f, -1000.0f }, (Vector3){ 0.0f, 0.0f, 1000.0f }, BLUE);
}
