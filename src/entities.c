#include "entities.h"
#include "rcamera.h"

BoundingBox CreateHitbox(const Vector3 position, const Vector3 size, const Vector3 padding) {
    return (BoundingBox) {
        (Vector3) {
            position.x - size.x / 2.0f - padding.x,
            position.y - size.y / 2.0f - padding.y,
            position.z - size.z / 2.0f - padding.z
        },
        (Vector3) {
            position.x + size.x / 2.0f + padding.x,
            position.y + size.y / 2.0f + padding.y,
            position.z + size.z / 2.0f + padding.z
        }
    };
}

void MovePlayer(Player* const player) {
    // W, A, S, D to move the player's position
    float speed = player->speed;
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 2.0f;
    }
    speed *= GetFrameTime();

    if (IsKeyDown(KEY_A)) { // left
        CameraMoveRight(&player->camera, -speed, true);
    } else if (IsKeyDown(KEY_D)) { // right
        CameraMoveRight(&player->camera, speed, true);
    }

    if (IsKeyDown(KEY_W)) { // front
        CameraMoveForward(&player->camera, speed, true);
    } else if (IsKeyDown(KEY_S)) { // back
        CameraMoveForward(&player->camera, -speed, true);
    }

    // move the mouse to look up/down/left/right
    const Vector2 delta = GetMouseDelta();
    const float sensitivity = 0.01f * GetFrameTime();
    CameraPitch(&player->camera, -delta.y * sensitivity, true, false, false);
    CameraYaw(&player->camera, -delta.x * sensitivity, false);

    player->hitbox = CreateHitbox(player->camera.position, player->size, player->hitboxPadding);
}
