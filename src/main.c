#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "Context.h"
#include "utils.h"

#include "vertexBuffer.h"
#include "draw.h"

Context context;

typedef struct 
{
    Vector3d position;
    Color color;
} Vertex;

int main(int argc, char** argv)
{
    constructContext(&context);

    Vertex data[3] = {
        {{ 0.0,  0.5, 0.}, {255, 0, 0, 255}},
        {{-0.5, -0.5, 0.}, {0, 255, 0, 255}},
        {{ 0.5, -0.5, 0.}, {0, 0, 255, 255}}
    };

    // size_t index[3] = {
    //     0, 1, 2
    // };

    VertexBuffer* vertexBuffer = newVertexBuffer(data, sizeof(Vertex), 3);
    VertexBufferConfigureAttribute(vertexBuffer, 0, 3, TYPE_DOUBLE, offsetof(Vertex, position));
    VertexBufferConfigureAttribute(vertexBuffer, 1, 4, TYPE_UINT8, offsetof(Vertex, color));

    // indexBuffer indexBuffer = newIndexBuffer(index, sizeof(size_t) * 3);

    // drawIndexBuffer(indexBuffer, vertexBuffer, context.renderer);
    drawVertexBuffer(
        DRAW_MODE_TRIANGLES, 0, 3, vertexBuffer
    );

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

