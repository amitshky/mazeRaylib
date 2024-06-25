#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "rcamera.h"

void CameraController(Camera3D* const camera);

int main(void) {
    InitWindow(640, 640, "Cube");

    Camera3D camera = {
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

        BeginMode3D(camera);

        DrawCubeV((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 40.0f, 40.0f, 40.0f }, RED);

        DrawLine3D((Vector3){ 0.0f, -1000.0f, 0.0f }, (Vector3){ 0.0f, 1000.0f, 0.0f }, GREEN);
        DrawLine3D((Vector3){ -1000.0f, 0.0f, 0.0f }, (Vector3){ 1000.0f, 0.0f, 0.0f }, RED);
        DrawLine3D((Vector3){ 0.0f, 0.0f, -1000.0f }, (Vector3){ 0.0f, 0.0f, 1000.0f }, BLUE);

        EndMode3D();

        EndDrawing();
        CameraController(&camera);
    }

    CloseWindow();
    return 0;
}

void CameraController(Camera3D* const camera) {
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
    const float distance = 70.0f * GetFrameTime();

    if (IsKeyDown(KEY_A)) { // left
        CameraMoveRight(camera, -distance, true);
    } else if (IsKeyDown(KEY_D)) { // right
        CameraMoveRight(camera, distance, true);
    }

    if (IsKeyDown(KEY_W)) { // front
        CameraMoveForward(camera, distance, true);
    } else if (IsKeyDown(KEY_S)) { // back
        CameraMoveForward(camera, -distance, true);
    }

    if (IsKeyDown(KEY_E)) { // up
        CameraMoveUp(camera, distance);
    } else if (IsKeyDown(KEY_Q)) { // back
        CameraMoveUp(camera, -distance);
    }

    // move the mouse to look up/down/left/right
    const Vector2 delta = GetMouseDelta();
    const float sensitivity = 0.01f * GetFrameTime();
    CameraPitch(camera, -delta.y * sensitivity, true, false, false);
    CameraYaw(camera, -delta.x * sensitivity, false);
}
