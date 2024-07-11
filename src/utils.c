#include "utils.h"
#include <stdio.h>

void DrawGuides(void) {
    const float bounds = 1000.0f;
    DrawLine3D((Vector3){ 0.0f, -bounds, 0.0f }, (Vector3){ 0.0f, bounds, 0.0f }, GREEN);
    DrawLine3D((Vector3){ -bounds, 0.0f, 0.0f }, (Vector3){ bounds, 0.0f, 0.0f }, RED);
    DrawLine3D((Vector3){ 0.0f, 0.0f, -bounds }, (Vector3){ 0.0f, 0.0f, bounds }, BLUE);
}

void DrawCrosshair(void) {
    const int width  = GetScreenWidth();
    const int height = GetScreenHeight();
    const float widthHalf  = (float)width * 0.5f;
    const float heightHalf = (float)height * 0.5f;

    const float centerRadius = 4.0f;
    const float length = 15.0f;
    const float breadthHalf = 1.0f;
    const float breadth = breadthHalf * 2.0f;

    const Color color = GetColor(0xe8e7e5e0);
    const Vector2 sizeVert = (Vector2) { breadth, length };
    const Vector2 sizeHorz = (Vector2) { length, breadth };

    DrawRectangleV((Vector2) { widthHalf - breadthHalf          , heightHalf - centerRadius - length }, sizeVert, color); // top
    DrawRectangleV((Vector2) { widthHalf + centerRadius         , heightHalf - breadthHalf           }, sizeHorz, color); // right
    DrawRectangleV((Vector2) { widthHalf - breadthHalf          , heightHalf + centerRadius          }, sizeVert, color); // bottom
    DrawRectangleV((Vector2) { widthHalf - centerRadius - length, heightHalf - breadthHalf           }, sizeHorz, color); // left
}

bool IsButtonHovered(Button button) {
    Vector2 mousePos = GetMousePosition();
    if (mousePos.x >= button.position.x && mousePos.x <= (button.position.x + button.size.x)
            && mousePos.y >= button.position.y && mousePos.y <= (button.position.y + button.size.y))
        return true;

    return false;
}

bool IsButtonClicked(Button button) {
    if (IsButtonHovered(button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return true;

    return false;
}

void DrawButton(Button button) {
    Vector2 padding = { 10.0f, 10.0f };
    int fontSize = (int)(button.size.y - 2.0f * padding.y);
    int textPosX = (int)(button.position.x + padding.x);
    int textPosY = (int)(button.position.y + padding.y);

    if (IsButtonHovered(button)) {
        button.bgColor = ColorAlpha(button.bgColor, 0.6f);
        button.fgColor = ColorAlpha(button.fgColor, 0.8f);
    }

    DrawRectangleV(button.position, button.size, button.bgColor);
    DrawRectangleLines((int)button.position.x, (int)button.position.y, (int)button.size.x, (int)button.size.y, button.borderColor);
    DrawText(button.text, textPosX, textPosY, fontSize, button.fgColor);
}
