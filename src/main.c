#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "application.h"

void UIDrawGuides();

int main(void) {
    InitWindow(640, 640, "Maze");
    SetTargetFPS(60);

    Application app = {
        .Init = Init,
        .Cleanup = Cleanup,
        .OnUpdate = OnUpdate,
        .LoadMap = LoadMap,
        .ControlCamera = ControlCamera,
    };
    app.Init(&app, "assets/map/map2.txt");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(app.camera);

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
    const float bounds = 1000.0f;
    DrawLine3D((Vector3){ 0.0f, -bounds, 0.0f }, (Vector3){ 0.0f, bounds, 0.0f }, GREEN);
    DrawLine3D((Vector3){ -bounds, 0.0f, 0.0f }, (Vector3){ bounds, 0.0f, 0.0f }, RED);
    DrawLine3D((Vector3){ 0.0f, 0.0f, -bounds }, (Vector3){ 0.0f, 0.0f, bounds }, BLUE);
}
