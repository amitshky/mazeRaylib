#pragma once

#include "raylib.h"
#include "types.h"

#define ARRAY_LEN(x) sizeof(x) / sizeof(x[0])

void ToggleCursorVisibility();

void DrawGuides(void);
void DrawCrosshair(void);

// ui
bool IsButtonHovered(Button button);
bool IsButtonClicked(Button button);
void DrawButton(Button button);
