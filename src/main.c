#define SDL_MAIN_HANDLED
#include <time.h>
#include "Renderer.h"
#include "Context.h"
#include "utils.h"

#include "VertexBuffer.h"
#include "draw.h"
#include "Type.h"
#include "Shaders.h"

Context context;

typedef struct 
{
    Vector3d position;
    Vector3d color;
} Vertex;

void vertexShader(
    Uniforms* uniforms, void* p_vertex, VertexBuffer* vertexBuffer,
    void* outputBuffer
)
{
    Matrix4 MVP = *(Matrix4*) getUniform(uniforms, 0);

    Vector3d aPosition;
    Vector3d aColor;
    vertexShaderLoadAttributesFromVertexPointer(
        p_vertex, vertexBuffer,
        &aPosition, sizeof(aPosition),
        &aColor, sizeof(aColor)
    );

    Vector3d position = Vector4dHomogenousDivide(Matrix4MultiplyVector4dHomogeneous(
        &MVP, Vector3dToVector4dHomogenous(aPosition)
    ));
    Vector3d color = aColor;

    vertexShaderCopyToOutputBuffer(
        outputBuffer,
        &position, sizeof(position),
        &color, sizeof(color)
    );
}

void fragmentShader(void* interpolatedVSOutput, Color* color)
{
    Vector3d vecColor = \
        *(Vector3d*) vertexShaderOutputGetAttributePointerByIndex(interpolatedVSOutput, 1);
    *color = Vector4dToColor(Vector3dToVector4dHomogenous(vecColor));
}

int main(int argc, char** argv)
{
    constructContext(&context);

    // construct vertex buffer
    Vertex data[3] = {
        {{-0.5, -0.5, 0.}, {1.0, 0.0, 0.0}},
        {{ 0. ,  0.5, 0.}, {0.0, 1.0, 0.0}},
        {{ 0.5, -0.5, 0.}, {0.0, 0.0, 1.0}},
    };

    size_t attributeOffsets[2] = {
        offsetof(Vertex, position),
        offsetof(Vertex, color)
    };

    VertexBuffer* vertexBuffer = newVertexBuffer(
        data, sizeof(Vertex), 3, attributeOffsets, 2
    );

    // construct index buffer
    // size_t index[3] = {
    //     0, 1, 2
    // };
    
    // indexBuffer indexBuffer = newIndexBuffer(index, sizeof(size_t) * 3);

    // bind the shaders and vertex shader output information
    size_t vsOutputAttributeOffsets[2] = {0, sizeof(Vector3d)};
    Type vsOutputAttributeTypes[2] = {TypeVector3d, TypeVector3d};

    VertexShaderOutputInformation vsOutputInfo = {
        .nBytes = sizeof(Vector3d) + sizeof(Vector3d),
        .nAttributes = 2,
        .attributeOffsets = vsOutputAttributeOffsets,
        .attributeTypes = vsOutputAttributeTypes
    };

    context.vertexShader = vertexShader;
    context.vertexShaderOutputInformation = vsOutputInfo;
    context.fragmentShader = fragmentShader;

    Matrix4 identity = Matrix4ConstructIdentity();
    addUniform(context.uniforms, 0, &identity, sizeof(Matrix4));

    size_t frameCount = 0;
    clock_t begin, end;
    double frametimeSec;
    while (context.running)
    {
        begin = clock();

        // load the uniforms
        // Matrix4 rotation = Matrix4ConstructRotate(
        //     (Vector3d) {RADIANS(0.0), RADIANS(0.0), RADIANS(frameCount)}
        // );
        // modifyUniform(context.uniforms, 0, &rotation, sizeof(Matrix4));

        rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});
        drawVertexBuffer(vertexBuffer, DRAW_MODE_TRIANGLES, 0, 3);

        // drawIndexBuffer(indexBuffer, vertexBuffer, context.renderer);

        pollEvents();
        rendererSwapBuffer(context.renderer);

        frameCount++;
        end = clock();
        frametimeSec = (double) (end - begin) / CLOCKS_PER_SEC;
        LOGI("Frametime: %lf s; FPS: %lf; Framecount: %zu\n", frametimeSec, 1 / frametimeSec, frameCount);

        if (frameCount == 5000)
            break;
    }

    freeVertexBuffer(vertexBuffer);
    return 0;
}

