#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "types.h"
#include "application.h"

int main(void) {
    // TODO: hot reloading
    // TODO: draw ui (win screen, settings, restart, quit, etc)
    // TODO: map at the top left corner
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

        // UIDrawGuides();
        app.Update(&app);

        EndMode3D();

        app.UpdateOverlay(&app);
        DrawFPS(10, 10);
        EndDrawing();
    }

    app.Cleanup(&app);
    CloseWindow();
    return 0;
}
