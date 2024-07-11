#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "rcamera.h"
#include "raymath.h"

#include "camera.h"
#include "utils.h"


void Init(Application* const this, const Config config) {
    this->config = config;
    this->gameState = GAME;
    this->pauseScreenLoaded = false;

    this->mapLayout = NULL;
    this->entities = NULL;
    this->numMapLayout = 0;
    this->numEntities = 0;
    this->numWalls = 0;
    this->numEnemies = 0;

    this->LoadMap(this);

    this->sceneCamera = (Camera3D) {
        .position = { 75.0f, 200.0f, 76.5f },
        .target   = { 75.0f,   0.0f, 75.5f },
        .up       = {  0.0f,   1.0f,  0.0f },
        .fovy     = this->config.fovy,
        .projection = CAMERA_PERSPECTIVE,
    };

    // this will be toggled to PLAYER_CAMERA
    this->activeCamera = SCENE_CAMERA;
    this->ToggleActiveCamera(this);
}

void Cleanup(Application* const this) {
    free(this->mapLayout);
    this->mapLayout = NULL;
    this->numMapLayout = 0;

    free(this->entities);
    this->entities = NULL;
    this->numEntities = 0;
    this->numWalls = 0;
    this->numEnemies = 0;
}

void Update(Application* const this) {
    switch (this->gameState) {
        case GAME:
            if (!IsCursorHidden()) {
                HideCursor();
                DisableCursor();
            }
            this->UpdateGame(this);
            break;

        default: // go to other screens (pause, end)
            if (IsCursorHidden()) {
                ShowCursor();
                EnableCursor();
            }
            break;
    }

    // exit on Left CTRL+Q
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) {
        glfwSetWindowShouldClose(GetWindowHandle(), 1);
    }
}

void UpdateOverlay(Application* const this) {
    switch (this->gameState) {
        case GAME:
            // render number of enemies remaining
            char text[255] = {};
            sprintf(text, "Enemies remaining: %lu", this->numEnemies);
            DrawText(text, 10, 32, 20, WHITE);
            if (this->activeCamera == PLAYER_CAMERA) {
                DrawCrosshair();
            }
            break;

        case PAUSE:
            this->UpdatePauseScreen(this);
            break;

        case END:
            this->UpdateEndScreen(this);
            break;
    }
}

void UpdateGame(Application* const this) {
    // switch camera on `tab` key press
    if (IsKeyPressed(KEY_TAB)) {
        this->ToggleActiveCamera(this);
    }

    // set state before collision
    CollisionState state = {
        .position = this->player.position,
    };

    switch (this->activeCamera) {
        case PLAYER_CAMERA:
            MovePlayer(&this->player);
            break;

        case SCENE_CAMERA:
            this->ControlCamera(this->camera);
            break;
    }

    // press left mouse button to shoot
    if (this->activeCamera == PLAYER_CAMERA && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        float minDistance = FLT_MAX; // to hit the closest enemy
        Entity* pEntity = NULL;
        uint64_t entityIdx = 0; // index of the entity hit

        Ray ray = {
            .position = this->player.position,
            .direction = GetCameraForward(&this->player.camera),
        };

        for (uint64_t i = 0; i < this->numEntities; ++i) {
            RayCollision hitInfo = GetRayCollisionBox(ray, this->entities[i].hitbox);
            if (hitInfo.hit && hitInfo.distance <= minDistance) {
                minDistance = hitInfo.distance;
                pEntity = &this->entities[i];
                entityIdx = i;
            }
        }

        if (minDistance != FLT_MAX && pEntity->type == ENTITY_ENEMY) {
            pEntity->enemy.health -= this->player.damageVal;
            // remove the dead enemies
            if (pEntity->enemy.health <= 0.0f) {
                for (uint64_t i = entityIdx; i < this->numEntities; ++i) {
                    if (i + 1 < this->numEntities) {
                        this->entities[i] = this->entities[i + 1];
                    }
                }
                --this->numEnemies;
                --this->numEntities;
            }
        }
    }

    // render entities and check player collision
    for (uint64_t i = 0; i < this->numEntities; ++i) {
        if (CheckCollisionBoxes(this->player.hitbox, this->entities[i].hitbox)) {
            // FIXME: fix camera turn on side collision
            PlayerOnCollision(&this->player, &state);
        }

        DrawCubeV(this->entities[i].position, this->entities[i].size, this->entities[i].color);
        if (this->activeCamera == SCENE_CAMERA) {
            DrawBoundingBox(this->entities[i].hitbox, GREEN);
        } else if (this->activeCamera == PLAYER_CAMERA) {
            DrawCubeWiresV(this->entities[i].position, this->entities[i].size, BLUE);
        }
    }

    if (this->activeCamera == SCENE_CAMERA) {
        DrawCubeV(this->player.position, this->player.size, this->player.color);
        DrawBoundingBox(this->player.hitbox, GREEN);
    }

    if (this->numEnemies == 0) {
        this->gameState = END;
    }

    if (IsKeyReleased(KEY_ESCAPE)) {
        this->pauseScreenLoaded = false;
        this->gameState = PAUSE;
    }
}

void UpdatePauseScreen(Application* const this) {
    ClearBackground(MENU_BG_COLOR);
    const int yPos = 50;
    int count = 1;
    DrawText("GAME PAUSED!", 10, yPos * (count++), 30, WHITE);

    Button resumeButton = {
        .text = "Resume <Esc>",
        .position = (Vector2) { 10.0f, (float)(yPos * (count++)) }, 
        .size = (Vector2) { 260, 40 }, 
        .fgColor = WHITE, 
        .bgColor = GRAY, 
        .borderColor = WHITE 
    };

    Button restartButton = { 
        .text = "Restart <R>", 
        .position = (Vector2) { 10.0f, (float)(yPos * (count++)) }, 
        .size = (Vector2) { 260, 40 }, 
        .fgColor = WHITE, 
        .bgColor = GRAY, 
        .borderColor = WHITE 
    };

    Button quitButton = { 
        .text = "Quit <Q>", 
        .position = (Vector2) { 10.0f, (float)(yPos * (count++)) }, 
        .size = (Vector2) { 260, 40 }, 
        .fgColor = WHITE, 
        .bgColor = GRAY, 
        .borderColor = WHITE 
    };

    DrawButton(resumeButton);
    DrawButton(restartButton);
    DrawButton(quitButton);

    // let pause screen load so as to not overlap ESCAPE key pressed to open pause screen
    if (this->pauseScreenLoaded) {
        if (IsButtonClicked(resumeButton) || IsKeyReleased(KEY_ESCAPE)) {
            this->gameState = GAME;
        } else if (IsButtonClicked(restartButton) || IsKeyPressed(KEY_R)) {
            this->gameState = GAME;
            this->ParseMap(this);
        } else if (IsButtonClicked(quitButton) || IsKeyPressed(KEY_Q)) {
            glfwSetWindowShouldClose(GetWindowHandle(), 1);
        }
    }

    this->pauseScreenLoaded = true;
}

void UpdateEndScreen(Application* const this) {
    ClearBackground(MENU_BG_COLOR);
    const int yPos = 50;
    int count = 1;
    DrawText("LEVEL COMPLETED!", 10, yPos * (count++), 30, WHITE);

    Button restartButton = { 
        .text = "Restart <R>", 
        .position = (Vector2) { 10.0f, (float)(yPos * (count++)) }, 
        .size = (Vector2) { 260, 40 }, 
        .fgColor = WHITE, 
        .bgColor = GRAY, 
        .borderColor = WHITE 
    };

    Button quitButton = { 
        .text = "Quit <Q>", 
        .position = (Vector2) { 10.0f, (float)(yPos * (count++)) }, 
        .size = (Vector2) { 260, 40 }, 
        .fgColor = WHITE, 
        .bgColor = GRAY, 
        .borderColor = WHITE 
    };

    DrawButton(restartButton);
    DrawButton(quitButton);

    if (IsButtonClicked(restartButton) || IsKeyPressed(KEY_R)) {
        this->gameState = GAME;
        this->ParseMap(this);
    } else if (IsButtonClicked(quitButton) || IsKeyPressed(KEY_Q)) {
        glfwSetWindowShouldClose(GetWindowHandle(), 1);
    }
}

/**
 * loads map from file
 */
void LoadMap(Application* const this) {
    FILE* fp = fopen(this->config.mapPath, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", this->config.mapPath);
        exit(-1);
    }

    fseek(fp, 0l, SEEK_END); // seek to the end of file
    const uint64_t fileSize = (const uint64_t)ftell(fp);
    fseek(fp, 0l, 0); // seek to the start of the file

    this->mapLayout = (char*)malloc((fileSize + 1) * sizeof(char));
    fread(this->mapLayout, sizeof(char), fileSize, fp);
    this->mapLayout[fileSize] = '\0';

    this->numMapLayout = fileSize + 1;

    // reallocated later
    this->entities = (Entity*)malloc(fileSize * sizeof(Entity));

    this->ParseMap(this);

    // reallocate with actual num of entities
    const size_t bytes = this->numEntities * sizeof(Entity);
    Entity* temp = (Entity*)malloc(bytes);
    memcpy(temp, this->entities, bytes);
    free(this->entities);
    this->entities = temp;
    temp = NULL;

    if (fclose(fp) == EOF) {
        fprintf(stderr, "Error closing file: %s\n", this->config.mapPath);
        exit(-1);
    }
}

void ParseMap(Application* const this) {
    // to keep count
    this->numEntities = 0;
    this->numEnemies = 0;
    this->numWalls = 0;
    // for player, enemy, etc position
    float x = 0.0f;
    float z = 0.0f;
    for (uint64_t i = 0; i < this->numMapLayout; ++i) {
        char ch = this->mapLayout[i];
        if (ch == '\0') {
            break;
        } else if (ch == '\n') {
            x = 0.0f;
            z += 10.f;
        } else {
            if (ch == '#') { // wall
                this->entities[this->numEntities] = InitWall((Vector3) { x, 0.0f, z });
                ++this->numWalls;
                ++this->numEntities;
            } else if (ch == '>' || ch == '<' || ch == 'v' || ch == '^') { // player
                // the characters specify direction the player is facing
                this->player = InitPlayer((Vector3) { x, 0.0f, z }, this->config.fovy, ch);
            } else if (ch == 'e') { // enemy
                this->entities[this->numEntities] = InitEnemy((Vector3) { x, 0.0f, z }, 10.0f);
                ++this->numEnemies;
                ++this->numEntities;
            }
            x += 10.0f;
        }
    }
}

void ControlCamera(Camera3D* const camera) {
    // press and hold the right mouse button or middle to move the camera
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
        HideCursor();
        DisableCursor();
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) || IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
        ShowCursor();
        EnableCursor();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        CameraPan(camera);
    else if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        CameraOrbit(camera);
}

void ToggleActiveCamera(Application* const this) {
    switch (this->activeCamera) {
        case PLAYER_CAMERA:
            this->camera = &this->sceneCamera;
            this->activeCamera = SCENE_CAMERA;
            ShowCursor();
            EnableCursor();
            break;

        case SCENE_CAMERA:
            this->camera = &this->player.camera;
            this->activeCamera = PLAYER_CAMERA;
            HideCursor();
            DisableCursor();
            break;
    }
}
