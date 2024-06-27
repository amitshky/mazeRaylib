#include "application.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

void Init(Application* const self, const char* path) {
    self->playerPosition = (Vector3){ 0.0f, 0.0f, 0.0f };

    FILE* fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Error opening file: %s\n", path);
        exit(-1);
    }

    fseek(fp, 0l, SEEK_END); // seek to the end of file
    const uint64_t fileSize = ftell(fp);
    fseek(fp, 0l, 0); // seek to the start of the file

    self->mapLayout = (char*)malloc(fileSize + 1);

    int ch;
    int i = 0;

    // for player position
    float x = 0.0f;
    float z = 0.0f;

    while ((ch = fgetc(fp)) != EOF) {
        self->mapLayout[i++] = (char)ch;
        if (ch == 'x') {
            self->playerPosition.x = x;
            self->playerPosition.z = z;
        }

        x += 10.0f;
        if (ch == '\n') {
            x = 10.0f;
            z += 10.f;
        }
    }

    if (fclose(fp) == EOF) {
        printf("Error closing file: %s\n", path);
        exit(-1);
    }
}

void Cleanup(Application* const self) {
    free(self->mapLayout);
}

void OnUpdate(Application* const self) {
    float x = 0.0f;
    float z = 0.0f;
    for (uint64_t i = 0; i < strlen(self->mapLayout); ++i) {
        if (self->mapLayout[i] == '#') {
            DrawCubeV((Vector3){ x, 0.0f, z }, (Vector3){ 10.0f, 10.0f, 10.0f }, (Color){ 193, 116, 93, 255 });
            x += 10.0f;
        } else if (self->mapLayout[i] == '\n') {
            x = 0.0f;
            z += 10.0f;
        } else if (self->mapLayout[i] == '.' || self->mapLayout[i] == 'x') {
            x += 10.0f;
        }
    }
}
