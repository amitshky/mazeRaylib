#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "application.h"
// #include "camera.h"

void UIDrawGuides();

int main(void) {
    InitWindow(640, 640, "Maze");
    SetTargetFPS(60);

    Application app = {
        .Init = Init,
        .Cleanup = Cleanup,
        .OnUpdate = OnUpdate,
        .UpdatePlayerCamera = UpdatePlayerCamera,
    };
    app.Init(&app, "assets/map/map1.txt");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(app.playerCamera);

        UIDrawGuides();
        app.OnUpdate(&app);

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
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
