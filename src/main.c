#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "raylib.h"
#include "types.h"
#include "utils.h"
#include "application.h"

    // TODO: hot reloading
    // TODO: draw ui settings (mouse sensitivity, movement speed)
    // TODO: map at the top left corner
    // TODO: swtich/pick levels
    // TODO: enemies shoot at you

int main(void) {
    Config config = {
        .width = 640,
        .height = 640,
        .title = "Maze",
        .mapPath = "assets/map/box.txt",
        .fovy = 45.0f,
    };

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(config.width, config.height, config.title);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    Application app = CREATE_APPLICATION();
    app.Init(&app, config);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(*app.camera);

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
