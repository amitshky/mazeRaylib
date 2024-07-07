#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "types.h"
#include "application.h"

void UIDrawGuides();

int main(void) {
    Config config = {
        .width = 640,
        .height = 640,
        .title = "Maze",
        .mapPath = "assets/map/map1.txt",
        .fovy = 45.0f,
    };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(config.width, config.height, config.title);
    SetTargetFPS(60);

    Application app = CREATE_APPLICATION();
    app.Init(&app, &config);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(*app.camera);

        UIDrawGuides();
        app.OnUpdate(&app);

        EndMode3D();

        app.UpdateOverlay(&app);
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
