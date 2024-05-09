#include "triangle.h"
#include <stdint.h>
#define SDL_MAIN_HANDLED
#include <assert.h>
#include "Renderer.h"
#include "shaders.h"
#include "Context.h"
#include "Type.h"
#include "Color/Color.h"
#include "buffer.h"
#include "log.h"
#include "timer.h"
#include "utils.h"

Context context;

#pragma pack(push, 1)
struct Vertex
{
    double position[3];
    double color[3];
};
#pragma pack(pop)

struct Uniforms
{
    Matrix4 rotation;
};

void vertexShader
    (const ShaderProgram* sp, const Vertex* pVertex, VSOutput* pOutput);
void geometryShader
    (const ShaderProgram* sp, const VSOutput* pInput, GSOutput* pOutput);
void fragmentShader
    (const ShaderProgram* sp, const Interpolated* pFragment, Color* color);
void fragmentShaderZ
    (const ShaderProgram* sp, const Interpolated* pFragment, Color* color);

int main(int argc, char** argv)
{
    constructContext(&context);

    Vertex data[6] = {
        {.position = {-0.5, -0.5, 0}, .color = {1., 0., 0.}},
        {.position = { 0   , 0.5, 0}, .color = {0., 1., 0.}},
        {.position = { 0.5, -0.5, 0}, .color = {0., 0., 1.}},

        {.position = {0.5, -1, -0.9}, .color = {1., 1., 1.}},
        {.position = {0  ,  0,  0  }, .color = {1., 1., 1.}},
        {.position = {1  ,  0, -0.9}, .color = {1., 1., 1.}}
    };
    uint64_t indices[3] = {
        0, 2, 1,
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

    VertexBuffer* vb = \
        newVertexBuffer(sizeof(Vertex), sizeof(data), data, 2, attributes);
    IndexBuffer* ib = newIndexBuffer(TYPE_UINT64, sizeof(indices), indices);

    Uniforms uniforms;
    ShaderProgram shaderProgram = {
        .uniforms = &uniforms,
        .vertexShader = {
            .shader = vertexShader,
            .nBytesPerOutputVertex = sizeof(Vertex),
            .nOutputAttributes = 2,
            .outputAttributes = attributes,
            .indexOfOutputPositionAttribute = 0
        },
        .geometryShader = {
            .shader = NULL
            // .shader = geometryShader,
            // .nBytesPerOutputVertex = sizeof(Vertex),
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

        shaderProgram.uniforms->rotation = Matrix4ConstructRotate(
            (Vector3d) {RADIANS(0.0), RADIANS(0.0), RADIANS(frameCount)}
        );

        rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});
        drawIndexBuffer(ib, vb, PRIMITIVE_TRIANGLES, 0, 3, &shaderProgram);

        pollEvents();
        rendererSwapBuffer(context.renderer);

        frameCount++;
        TIMER_STOP(frametime);
        LOGI(
            "Frametime: %li us; FPS: %lf; Framecount: %zu\n", 
            TIMER_REPORT_US(frametime, long),
            1. / TIMER_REPORT_S(frametime, double),
            frameCount
        );
    }

    freeVertexBuffer(vb);
    freeIndexBuffer(ib);
    return 0;
}

void vertexShader
    (const ShaderProgram* sp, const Vertex* pVertex, VSOutput* pOutput)
{
    Vector4d position = {
        pVertex->position[0],
        pVertex->position[1],
        pVertex->position[2],
        1
    };
    Vector4d transformedPosition = Matrix4MultiplyVector4d(
        &sp->uniforms->rotation, position
    );
    memcpy(
        &((Vertex*) pOutput)->position, &transformedPosition,
        3 * sizeof(double)
    );
    memcpy(
        &((Vertex*) pOutput)->color, pVertex->color, 3 * sizeof(double)
    );
}

void geometryShader
    (const ShaderProgram* sp, const VSOutput* pInput, GSOutput* pOutput)
{
    VertexAttribute inputPositionAttribute = sp->vertexShader.outputAttributes[
        sp->vertexShader.indexOfOutputPositionAttribute
    ];

    assert(
        sp->vertexShader.nBytesPerOutputVertex == \
        sp->geometryShader.nBytesPerOutputVertex
    );
    assert(inputPositionAttribute.nItems == 3);
    assert(inputPositionAttribute.type == TYPE_DOUBLE);

    size_t nBytesPerInputVertex = sp->vertexShader.nBytesPerOutputVertex;
    for (uint8_t i = 0; i < 3; i++)
    {
        size_t inputVertexOffset = i * nBytesPerInputVertex;
        VSOutput* pInputVertex = (VSOutput*) VOID_PTR_ADD(pInput, inputVertexOffset);

        GSOutput* pOutputVertex[2] = {
            (GSOutput*) INDEX_VOID_PTR(pOutput, i, nBytesPerInputVertex),
            (GSOutput*) INDEX_VOID_PTR(pOutput, i+3, nBytesPerInputVertex)
        };

        double* inputPosition = (double*) VOID_PTR_ADD(pInputVertex, inputPositionAttribute.offsetBytes);
        double newPosition[3] = {
            inputPosition[0] - 0.1,
            inputPosition[1] + 0.1,
            inputPosition[2] - 0.1
        };

        memcpy(pOutputVertex[0], pInputVertex, nBytesPerInputVertex);
        memcpy(
            VOID_PTR_ADD(pOutputVertex[1], inputPositionAttribute.offsetBytes),
            newPosition,
            3 * sizeof(double)
        );
        memcpy(
            VOID_PTR_ADD(pOutputVertex[1], sp->geometryShader.outputAttributes[1].offsetBytes),
            VOID_PTR_ADD(pInputVertex, sp->vertexShader.outputAttributes[1].offsetBytes),
            sp->vertexShader.outputAttributes[1].nItems * sizeof(double)
        );
    }
}

void fragmentShader
    (const ShaderProgram* sp, const Interpolated* pFragment, Color* color)
{
    double* colorVec = ((Vertex*) pFragment)->color;
    *color = (Color) {
        round(colorVec[0] * 255),
        round(colorVec[1] * 255),
        round(colorVec[2] * 255),
        255
    };
}

void fragmentShaderZ
    (const ShaderProgram* sp, const Interpolated* pFragment, Color* color)
{
    double* position = ((Vertex*) pFragment)->position;
    *color = (Color) {
        (position[2] + 1) / 2 * 255,
        (position[2] + 1) / 2 * 255,
        (position[2] + 1) / 2 * 255,
        255
    };
}

