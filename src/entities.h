#pragma once

#include "raylib.h"

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Color color;

    Camera3D camera;

    Vector3 hitboxPadding;
    BoundingBox hitbox;
} Player;

typedef struct Wall {
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox hitbox;
} Wall;

BoundingBox CreateHitbox(const Vector3 position, const Vector3 size, const Vector3 padding);
void MovePlayer(Player* const player);
