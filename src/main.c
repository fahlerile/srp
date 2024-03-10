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

void vertexShader(void* sp, void* pVertex, void* pOutput)
{
    memcpy(pOutput, pVertex, ((ShaderProgram*) sp)->vertexShader.nBytesPerOutputVertex);
}

void fragmentShader(void* sp, void* pInterpolated, Color* color)
{
    Vector3d colorVec = *(Vector3d*) (
        (uint8_t*) pInterpolated + \
        ((ShaderProgram*) sp)->geometryShader.outputAttributes[1].offsetBytes
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
        {.position = {-0.75, -0.75,  0.0}, .color = {1., 0., 0.}},
        {.position = { 0.  ,  0.75,  0.0}, .color = {0., 1., 0.}},
        {.position = { 0.75, -0.75,  0.0}, .color = {0., 0., 1.}}
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
            .shader = NULL,
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

