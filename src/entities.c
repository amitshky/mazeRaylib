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
