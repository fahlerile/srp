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

void geometryShader(
    void* p_vertex, VertexBuffer* vertexBuffer, Uniforms* uniforms,
    Vector4d* transformedPositionHomogenous
)
{
    Vector3d position = \
        *(Vector3d*) VertexPointerGetAttributePointerByIndex(
            vertexBuffer, p_vertex, 0
        );
    *transformedPositionHomogenous = (Vector4d) {
        position.x,
        position.y,
        position.z,
        1.
    };
}

void fragmentShader()
{

}

int main(int argc, char** argv)
{
    constructContext(&context);

    Vertex data[3] = {
        {{ 0.0,  0.5, 0.}, {255, 0, 0, 255}},
        {{-0.5, -0.5, 0.}, {0, 255, 0, 255}},
        {{ 0.5, -0.5, 0.}, {0, 0, 255, 255}}
    };

    size_t attributeOffsets[2] = {
        offsetof(Vertex, position),
        offsetof(Vertex, color)
    };

    VertexBuffer* vertexBuffer = newVertexBuffer(
        data, sizeof(Vertex), 3, attributeOffsets, 2
    );

    drawVertexBuffer(
        DRAW_MODE_TRIANGLES, 0, 3, vertexBuffer,
        geometryShader, fragmentShader
    );

    // size_t index[3] = {
    //     0, 1, 2
    // };
    
    // indexBuffer indexBuffer = newIndexBuffer(index, sizeof(size_t) * 3);
    // drawIndexBuffer(indexBuffer, vertexBuffer, context.renderer);

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

