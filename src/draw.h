#pragma once

typedef enum
{
    DRAW_MODE_LINES = 0,
    DRAW_MODE_TRIANGLES
} DrawMode;

typedef void (*GeometryShaderType)(
    void* p_vertex, Uniforms* uniforms, VertexBuffer* vertexBuffer,
    Vector4d* transformedPositionHomogenous
);
typedef void (*FragmentShaderType)();

void drawVertexBuffer(
    DrawMode drawMode, size_t startIndex, size_t count, 
    VertexBuffer* vertexBuffer, GeometryShaderType geometryShader, 
    FragmentShaderType fragmentShader
);
static void drawTriangle(
    Vector3d* NDCPositions, VertexBuffer* vertexBuffer
);

