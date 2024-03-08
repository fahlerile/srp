#define SDL_MAIN_HANDLED
#include <time.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Shaders.h"
#include "Context.h"
#include "Type.h"
#include "log.h"

Context context;

typedef struct
{
    double position[3];
} Vertex;

void vertexShader(void* pVertex, void* pOutput)
{
    memcpy(pOutput, pVertex, sizeof(double) * 3);
}

int main(int argc, char** argv)
{
    constructContext(&context);

    Vertex data[3] = {
        {.position = {-0.5, -0.5,  0.0}},
        {.position = { 0.0,  0.5,  0.0}},
        {.position = { 0.5, -0.5,  0.0}}
    };
    VertexAttribute attributes[1] = {{
        .nItems = 3,
        .type = TYPE_DOUBLE,
        .offsetBytes = 0
    }};
    VertexBuffer* vb = newVertexBuffer(
        sizeof(Vertex), sizeof(data), data, 1, attributes
    );

    ShaderProgram shaderProgram = {
        .vertexShader = {
            .shader = vertexShader,
            .nBytesPerVertex = sizeof(double) * 3,
            .nAttributes = 1,
            .attributes = attributes,
            .indexOfPositionAttribute = 0
        },
        .geometryShader = {
            .shader = NULL,
        },
        .fragmentShader = {
            .shader = NULL
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
        // drawIndexBuffer();

        pollEvents();
        rendererSwapBuffer(context.renderer);

        frameCount++;
        end = clock();
        frametimeSec = (double) (end - begin) / CLOCKS_PER_SEC;
        LOGI(
            "Frametime: %lf s; FPS: %lf; Framecount: %zu\n", 
            frametimeSec, 1 / frametimeSec, frameCount
        );

        break;
        if (frameCount == 5000)
            break;
    }

    freeVertexBuffer(vb);
    return 0;
}

