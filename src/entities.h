#pragma once

#include <stdint.h>
#include <stdlib.h>
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

// `playerDirectionChar` - get this from the map file
Player InitPlayer(const Vector3 position, const float fovy, const char playerDirectionChar);
void MovePlayer(Player* const player);
void PlayerOnCollision(Player* const player, const CollisionState* const state);
void PlayerShoot(Player* const player, Entity* const entities, uint64_t* const numEntities, uint64_t* const numEnemies);

Entity InitWall(const Vector3 position);
Entity InitEnemy(const Vector3 position, const float health);

void RemoveEntityElementAt(Entity* const entities, uint64_t* const num, uint64_t index); // remove entity from a array of entities
