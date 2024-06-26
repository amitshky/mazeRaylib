#include "application.h"
#include <stdint.h>
#include <string.h>
#include "utils.h"

void OnUpdate(Application* const self) {
    float x = 0.0;
    float z = 0.0;
    for (uint64_t i = 0; i < strlen(self->mapLayout); ++i) {
        if (self->mapLayout[i] == '#') {
            DrawCubeV((Vector3){ x, 0.0f, z }, (Vector3){ 10.0f, 10.0f, 10.0f }, RED);
            x += 15.0f;
        } else if (self->mapLayout[i] == '\n') {
            x = 0.0f;
            z += 15.0f;
        } else {
            x += 15.0f;
        }
    }
}

// void Init(Application* const self) {}

// void Cleanup(Application* const self) {}
