#pragma once

#include "raylib.h"

typedef struct Player {
    // the camera position is the position of the player
    Vector3 size;
    Color color;

    Camera3D camera;
    float speed;
    Vector3 direction; // direction to look at

    Vector3 hitboxPadding;
    BoundingBox hitbox;
} Player;

typedef struct Wall {
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox hitbox;
} Wall;

typedef struct Enemy {
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox hitbox;

    float health;
} Enemy;

BoundingBox CreateHitbox(const Vector3 position, const Vector3 size, const Vector3 padding);
void MovePlayer(Player* const player);
