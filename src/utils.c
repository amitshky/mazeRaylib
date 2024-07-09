#include "utils.h"

void UIDrawGuides(void) {
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
