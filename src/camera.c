#include "camera.h"
#include "rcamera.h"

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
    const float distance = 90.0f * GetFrameTime();

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
