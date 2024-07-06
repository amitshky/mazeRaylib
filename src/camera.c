#include "camera.h"
#include "rcamera.h"
#include "raymath.h"

void CameraPan(Camera3D* const camera) {
    // W, A, S, D, E, Q to move the camera's position
    float speed = 90.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 0.2f;
    } else if (IsKeyDown(KEY_LEFT_CONTROL)) {
        speed *= 5.0f;
    }
    speed *= GetFrameTime();

    if (IsKeyDown(KEY_A)) { // left
        CameraMoveRight(camera, -speed, true);
    } else if (IsKeyDown(KEY_D)) { // right
        CameraMoveRight(camera, speed, true);
    }

    if (IsKeyDown(KEY_W)) { // front
        CameraMoveForward(camera, speed, true);
    } else if (IsKeyDown(KEY_S)) { // back
        CameraMoveForward(camera, -speed, true);
    }

    if (IsKeyDown(KEY_E)) { // up
        CameraMoveUp(camera, speed);
    } else if (IsKeyDown(KEY_Q)) { // back
        CameraMoveUp(camera, -speed);
    }

    // move the mouse to look up/down/left/right
    const Vector2 delta = GetMouseDelta();
    const float sensitivity = 0.01f * GetFrameTime();
    CameraPitch(camera, -delta.y * sensitivity, true, false, false);
    CameraYaw(camera, -delta.x * sensitivity, false);
}

void CameraOrbit(Camera3D* const camera) {
    const Vector2 delta = Vector2Normalize(GetMouseDelta());
    const float speed = 5.0f * delta.x * GetFrameTime();

    Matrix rotation = MatrixRotate(GetCameraUp(camera), speed);

    Vector3 view = Vector3Subtract(camera->position, camera->target);
    view = Vector3Transform(view, rotation);
    camera->position = Vector3Add(camera->target, view);
}
