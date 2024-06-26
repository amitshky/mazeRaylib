#include <stdio.h>
#include "raylib.h"
#include "application.h"
#include "camera.h"

void UIDrawGuides();

int main(void) {
    InitWindow(640, 640, "Cube");

    Application app = {
        // .Init = Init,
        // .Cleanup = Cleanup,
        .OnUpdate = OnUpdate,
        .mapLayout = "###   #\n# #",
    };

    Camera3D playerCamera = {
        .position = { 100.0f, 100.0f, 200.0f },
        .target = { 0.0f, 0.0f, 0.0f },
        .up = { 0.0f, 1.0f, 0.0f },
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(playerCamera);

        UIDrawGuides();
        app.OnUpdate(&app);

        EndMode3D();

        EndDrawing();
        CameraController(&playerCamera);
    }

    CloseWindow();
    return 0;
}


void UIDrawGuides() {
    DrawLine3D((Vector3){ 0.0f, -1000.0f, 0.0f }, (Vector3){ 0.0f, 1000.0f, 0.0f }, GREEN);
    DrawLine3D((Vector3){ -1000.0f, 0.0f, 0.0f }, (Vector3){ 1000.0f, 0.0f, 0.0f }, RED);
    DrawLine3D((Vector3){ 0.0f, 0.0f, -1000.0f }, (Vector3){ 0.0f, 0.0f, 1000.0f }, BLUE);
}
