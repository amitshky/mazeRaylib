#include "entities.h"
#include "raymath.h"
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

Player InitPlayer(const Vector3 position, const float fovy, const char playerDirectionChar) {
    Player p = (Player) {
        .position = position,
        .size     = { 4.0f, 10.0f, 4.0f },
        .color    = WHITE,
        .hitboxPadding = { 0.5f, 0.0f, 0.5f },
        .speed     = 15.0f,
        .damageVal = 10.0f,
        .camera = {
            .position = p.position,
            .target   = { 0.0f, 0.0f, 0.0f },
            .up       = { 0.0f, 1.0f, 0.0f },
            .fovy     = fovy,
            .projection = CAMERA_PERSPECTIVE,
        },
    };

    p.hitbox = CreateHitbox(p.position, p.size, p.hitboxPadding);

    switch (playerDirectionChar) {
        case '<': // negative x-axis
            p.direction = (Vector3) { -1.0f, 0.0f, 0.0f };
            break;
        case '>': // positive x-axis
            p.direction = (Vector3) { 1.0f, 0.0f, 0.0f };
            break;
        case 'v': // positive z-axis
            p.direction = (Vector3) { 0.0f, 0.0f, 1.0f };
            break;
        case '^': // negative z-axis
            p.direction = (Vector3) { 0.0f, 0.0f, -1.0f };
            break;
    }
    p.camera.target = Vector3Add(p.position, p.direction);

    return p;
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

    player->position = player->camera.position;
    player->hitbox = CreateHitbox(player->position, player->size, player->hitboxPadding);
}

void PlayerOnCollision(Player* const player, const CollisionState* const state) {
    player->position = state->position;
    player->camera.position = player->position;
}

Entity InitWall(const Vector3 position) {
    Entity e = (Entity) {
        .type     = ENTITY_WALL,
        .wall     = {},
        .position = position,
        .size     = { 10.0f, 10.0f, 10.0f },
        .color    = GetColor(0x291a59ff),
        .hitboxPadding = Vector3Zero(),
    };
    e.hitbox = CreateHitbox(e.position, e.size, e.hitboxPadding);

    return e;
}

Entity InitEnemy(const Vector3 position, const float health) {
    Entity e = (Entity) {
        .type     = ENTITY_ENEMY,
        .position = position,
        .size     = { 6.0f, 6.0f, 6.0f },
        .color    = RED,
        .hitboxPadding = Vector3Zero(),
        .enemy = {
            .health = health,
        },
    };
    e.hitbox = CreateHitbox(e.position, e.size, e.hitboxPadding);

    return e;
}
