#define SDL_MAIN_HANDLED
#include <time.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Shaders.h"
#include "Context.h"
#include "Type.h"
#include "log.h"

Context context;

#pragma pack(push, 1)
typedef struct
{
    double position[3];
    double color[3];
} Vertex;
#pragma pack(pop)

void vertexShader(void* shaderProgram, void* pVertex, void* pOutput)
{
    ShaderProgram* sp = (ShaderProgram*) shaderProgram;
    memcpy(pOutput, pVertex, sp->vertexShader.nBytesPerOutputVertex);
}

void geometryShader(void* shaderProgram, void* pInput, void* pOutput)
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

void fragmentShader(void* shaderProgram, void* pInterpolated, Color* color)
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

int main(int argc, char** argv)
{
    constructContext(&context);
    
    Vertex data[3] = {
        {.position = {-0.5 , -0.25,  0.0}, .color = {1., 0., 0.}},
        {.position = {-0.25,  0.25,  0.0}, .color = {0., 1., 0.}},
        {.position = { 0.  , -0.25,  0.0}, .color = {0., 0., 1.}}
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

    VertexBuffer* vb = newVertexBuffer(
        sizeof(Vertex), sizeof(data), data, 1, attributes
    );

    ShaderProgram shaderProgram = {
        .vertexShader = {
            .shader = vertexShader,
            .nBytesPerOutputVertex = sizeof(double) * 6,
            .nOutputAttributes = 2,
            .outputAttributes = attributes,
            .indexOfOutputPositionAttribute = 0
        },
        .geometryShader = {
            .shader = geometryShader,
            .nBytesPerOutputVertex = sizeof(double) * 6,
            .nOutputAttributes = 2,
            .outputAttributes = attributes,
            .indexOfOutputPositionAttribute = 0,
            .nOutputVertices = 6,
            .inputPrimitive = PRIMITIVE_TRIANGLES,
            .outputPrimitive = PRIMITIVE_TRIANGLES
        },
        .fragmentShader = {
            .shader = fragmentShader
        }
    };

    size_t frameCount = 0;
    clock_t begin, end;
    double frametimeSec;
    while (context.running)
    {
        begin = clock();

        rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});
        drawVertexBuffer(vb, PRIMITIVE_TRIANGLES, 0, 3, &shaderProgram);

        pollEvents();
        rendererSwapBuffer(context.renderer);

        frameCount++;
        end = clock();
        frametimeSec = (double) (end - begin) / CLOCKS_PER_SEC;
        LOGI(
            "Frametime: %lf s; FPS: %lf; Framecount: %zu\n", 
            frametimeSec, 1 / frametimeSec, frameCount
        );
    }

    freeVertexBuffer(vb);
    return 0;
}

