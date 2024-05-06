#include "triangle.h"
#include <stdint.h>
#define SDL_MAIN_HANDLED
#include <assert.h>
#include "Renderer.h"
#include "Shaders.h"
#include "Context.h"
#include "Type.h"
#include "Color/Color.h"
#include "buffer.h"
#include "log.h"
#include "timer.h"

Context context;

#pragma pack(push, 1)
struct Vertex
{
    double position[3];
    double color[3];
};
#pragma pack(pop)

void vertexShader(const ShaderProgram* shaderProgram, const Vertex* pVertex, VSOutput* pOutput)
{
    ShaderProgram* sp = (ShaderProgram*) shaderProgram;
    memcpy(pOutput, pVertex, sp->vertexShader.nBytesPerOutputVertex);
}

void geometryShader(const ShaderProgram* shaderProgram, const VSOutput* pInput, GSOutput* pOutput)
{
    ShaderProgram* sp = (ShaderProgram*) shaderProgram;

    VertexAttribute inputPosition = sp->vertexShader.outputAttributes[
        sp->vertexShader.indexOfOutputPositionAttribute
    ];

    assert(
        sp->vertexShader.nBytesPerOutputVertex == \
        sp->geometryShader.nBytesPerOutputVertex
    );
    assert(inputPosition.nItems == 3 && inputPosition.type == TYPE_DOUBLE);

    size_t nBytesPerVertex = sp->vertexShader.nBytesPerOutputVertex;
    
    for (uint8_t i = 0; i < 3; i++)
    {
        size_t inputVertexOffset = i * nBytesPerVertex;
        void* pInputVertex = (uint8_t*) pInput + inputVertexOffset;

        void* pOutputVertex[2] = {
            (uint8_t*) pOutput + i * nBytesPerVertex,
            (uint8_t*) pOutput + (i+3) * nBytesPerVertex
        };

        double* oldPosition = (double*) ((uint8_t*) pInputVertex + inputPosition.offsetBytes);
        double newPosition[3] = {
            oldPosition[0] + 0.5,
            oldPosition[1],
            oldPosition[2]
        };

        memcpy(pOutputVertex[0], pInputVertex, nBytesPerVertex);
        memcpy(
            (uint8_t*) pOutputVertex[1] + inputPosition.offsetBytes,
            newPosition,
            inputPosition.nItems * sizeof(double)
        );
        memcpy(
            (uint8_t*) pOutputVertex[1] + sp->geometryShader.outputAttributes[1].offsetBytes,
            (uint8_t*) pInputVertex + sp->vertexShader.outputAttributes[1].offsetBytes,
            sp->vertexShader.outputAttributes[1].nItems * sizeof(double)
        );
    }
}

void fragmentShader(const ShaderProgram* shaderProgram, const Interpolated* pInterpolated, Color* color)
{
    ShaderProgram* sp = (ShaderProgram*) shaderProgram;
    Vector3d colorVec = *(Vector3d*) (
        (uint8_t*) pInterpolated + \
        sp->geometryShader.outputAttributes[1].offsetBytes
    );
    *color = (Color) {
        round(colorVec.x * 255),
        round(colorVec.y * 255),
        round(colorVec.z * 255),
        255
    };
}

void fragmentShaderZ(const ShaderProgram* shaderProgram, const Interpolated* pInterpolated, Color* color)
{
    ShaderProgram* sp = (ShaderProgram*) shaderProgram;
    double* position = (double*) ((uint8_t*) pInterpolated + sp->geometryShader.outputAttributes[
        sp->geometryShader.indexOfOutputPositionAttribute
    ].offsetBytes);
    *color = (Color) {
        (position[2] + 1) / 2 * 255,
        (position[2] + 1) / 2 * 255,
        (position[2] + 1) / 2 * 255,
        255
    };
}

int main(int argc, char** argv)
{
    constructContext(&context);
    
    Vertex data[6] = {
        {.position = {-0.75, -0.75, 0}, .color = {1., 0., 0.}},
        {.position = { 0   ,  0.75, 0}, .color = {0., 1., 0.}},
        {.position = { 0.75, -0.75, 0}, .color = {0., 0., 1.}},

        {.position = {0.5, -1, -1}, .color = {1., 1., 1.}},
        {.position = {0  ,  0,  1}, .color = {1., 1., 1.}},
        {.position = {1  ,  0, -1}, .color = {1., 1., 1.}}
    };
    uint64_t indices[6] = {
        0, 4, 1,
        2, 5, 3
    };
    VertexAttribute attributes[2] = {
        {
            .nItems = 3,
            .type = TYPE_DOUBLE,
            .offsetBytes = 0
        },
        {
            .nItems = 3,
            .type = TYPE_DOUBLE,
            .offsetBytes = sizeof(double) * 3
        }
    };

    VertexBuffer* vb = newVertexBuffer(sizeof(Vertex), sizeof(data), data, 2, attributes);
    IndexBuffer* ib = newIndexBuffer(TYPE_UINT64, sizeof(indices), indices);

    ShaderProgram shaderProgram = {
        .vertexShader = {
            .shader = vertexShader,
            .nBytesPerOutputVertex = sizeof(double) * 6,
            .nOutputAttributes = 2,
            .outputAttributes = attributes,
            .indexOfOutputPositionAttribute = 0
        },
        .geometryShader = {
            .shader = NULL
            // .shader = geometryShader,
            // .nBytesPerOutputVertex = sizeof(double) * 6,
            // .nOutputAttributes = 2,
            // .outputAttributes = attributes,
            // .indexOfOutputPositionAttribute = 0,
            // .nOutputVertices = 6,
            // .inputPrimitive = PRIMITIVE_TRIANGLES,
            // .outputPrimitive = PRIMITIVE_TRIANGLES
        },
        .fragmentShader = {
            .shader = fragmentShader
        }
    };
    shaderProgramSetDefaultGeometryShader(&shaderProgram);

    size_t frameCount = 0;
    while (context.running)
    {
        TIMER_START(frametime);

        rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});
        drawIndexBuffer(ib, vb, PRIMITIVE_TRIANGLES, 0, 6, &shaderProgram);

        pollEvents();
        rendererSwapBuffer(context.renderer);

        frameCount++;
        TIMER_STOP(frametime);
        LOGI(
            "Frametime: %li us; FPS: %lf; Framecount: %zu\n", 
            TIMER_REPORT_US(frametime, long), 1. / TIMER_REPORT_S(frametime, double),
            frameCount
        );
    }

    freeVertexBuffer(vb);
    freeIndexBuffer(ib);
    return 0;
}

