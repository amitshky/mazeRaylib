#pragma once

#include "raylib.h"
#include "types.h"

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Color color;
    Vector3 hitboxPadding;
    BoundingBox hitbox;
    Camera3D camera;

    float speed;
    Vector3 direction; // direction to look at
    float damageVal;
} Player;

typedef struct Wall {
} Wall;

typedef struct Enemy {
    float health;
} Enemy;

typedef struct Entity {
    enum Type {
        ENTITY_WALL,
        ENTITY_ENEMY,
    } Type;

    // common properies
    enum Type type;
    Vector3 position;
    Vector3 size;
    Color color;
    Vector3 hitboxPadding;
    BoundingBox hitbox;

    // entities
    union {
        Wall wall;
        Enemy enemy;
    };
} Entity;

BoundingBox CreateHitbox(const Vector3 position, const Vector3 size, const Vector3 padding);
void MovePlayer(Player* const player);
void PlayerOnCollision(Player* const player, const CollisionState* const state);
