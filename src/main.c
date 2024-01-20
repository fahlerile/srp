#define SDL_MAIN_HANDLED
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
    // layout(location = 0) uniform mat4 MVP;
    Matrix4 MVP = *(Matrix4*) getUniform(uniforms, 0);

    // layout(location = 0) in vec3 aPosition; 
    Vector3d aPosition = *(Vector3d*) VertexPointerGetAttributePointerByIndex(
        vertexBuffer, p_vertex, 0
    );
    Vector3d aColor = *(Vector3d*) VertexPointerGetAttributePointerByIndex(
        vertexBuffer, p_vertex, 1
    );

    Vector3d position = Vector4dHomogenousDivide(Matrix4MultiplyVector4dHomogeneous(
        &MVP, Vector3dToVector4dHomogenous(aPosition)
    ));
    Vector3d color = aColor;

    // Copy everything to the output buffer
    memcpy(
        indexVoidPointer(
            outputBuffer, context.vertexShaderOutputInformation.attributeOffsets[0], 1
        ),
        &position, sizeof(position)
    );
    memcpy(
        indexVoidPointer(
            outputBuffer, context.vertexShaderOutputInformation.attributeOffsets[1], 1
        ),
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

    Matrix4 rotation = Matrix4ConstructRotate(
        (Vector3d) {RADIANS(0.0), RADIANS(0.0), RADIANS(45.0)}
    );
    addUniform(context.uniforms, 0, &rotation, sizeof(Matrix4));

    rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});

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
    drawVertexBuffer(vertexBuffer, DRAW_MODE_TRIANGLES, 0, 3);

    // size_t index[3] = {
    //     0, 1, 2
    // };
    
    // indexBuffer indexBuffer = newIndexBuffer(index, sizeof(size_t) * 3);
    // drawIndexBuffer(indexBuffer, vertexBuffer, context.renderer);

    rendererSaveBuffer(context.renderer, "screenshot.bmp");
    rendererSwapBuffer(context.renderer);
    while (context.running)
    {
        pollEvents();
    }

    freeVertexBuffer(vertexBuffer);

    // Matrix4 viewMatrix = Matrix4ConstructView(
    //     (Vector3d) {0, 0, 0},
    //     (Vector3d) {RADIANS(0), RADIANS(0), RADIANS(0)},
    //     (Vector3d) {1, 1, 1}
    // );  
    // Matrix4 projectionMatrix = Matrix4ConstructPerspectiveProjection(
    //     -1, 1,
    //     -1, 1,
    //      1, 50
    // );

    // Scene* world = newScene(viewMatrix, projectionMatrix);
    // Model* teapot = newModel("res/models/utah_teapot.obj");
    // 
    // modelAddInstance(teapot, 
    //     (Vector3d) {0, 0, 20},
    //     (Vector3d) {RADIANS(0), RADIANS(0), RADIANS(0)},
    //     (Vector3d) {1, 1, 1}
    // );
    // sceneAddModel(world, teapot);
    // 
    // rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});
    // sceneRender(world);

    // rendererSaveBuffer(context.renderer, "screenshot.bmp");
    // rendererSwapBuffer(context.renderer);
    // while (context.running)
    // {
    //     pollEvents();
    //     #ifndef __linux__
    //         rendererSwapBuffer(renderer);
    //     #endif
    // }

    // freeSceneAndModels(world);

    return 0;
}

