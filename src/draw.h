#pragma once

typedef enum
{
    DRAW_MODE_LINES = 0,
    DRAW_MODE_TRIANGLES
} DrawMode;

void drawVertexBuffer(
    DrawMode drawMode, size_t startIndex, size_t count, 
    VertexBuffer* vertexBuffer
);
static void drawTriangle(
    Vector3d* NDCPositions, VertexBuffer* vertexBuffer
);

