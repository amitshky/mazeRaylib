#include "entities.h"

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
    float speed = 40.0f * GetFrameTime();
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        speed *= 0.1f;
    }

    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= speed;
    } else if (IsKeyDown(KEY_RIGHT)) {
        player->position.x += speed;
    }

    if (IsKeyDown(KEY_UP)) {
        player->position.z -= speed;
    } else if (IsKeyDown(KEY_DOWN)) {
        player->position.z += speed;
    }

    player->hitbox = CreateHitbox(player->position, player->size, player->hitboxPadding);
    player->camera.position = player->position;
}
