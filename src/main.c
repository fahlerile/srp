#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "Context.h"
#include "utils.h"

Context context;

int main(int argc, char** argv)
{
    constructContext(&context);

    Matrix4 viewMatrix = Matrix4ConstructView(
        (Vector3d) {0, 0, 0},
        (Vector3d) {RADIANS(0), RADIANS(0), RADIANS(0)},
        (Vector3d) {1, 1, 1}
    );  
    Matrix4 projectionMatrix = Matrix4ConstructPerspectiveProjection(
        -1, 1,
        -1, 1,
         1, 50
    );

    Scene* world = newScene(viewMatrix, projectionMatrix);
    Model* teapot = newModel("res/models/utah_teapot.obj");
    
    modelAddInstance(teapot, 
        (Vector3d) {0, 0, 20},
        (Vector3d) {RADIANS(0), RADIANS(0), RADIANS(0)},
        (Vector3d) {1, 1, 1}
    );
    sceneAddModel(world, teapot);
    
    rendererClearBuffer(context.renderer, (Color) {0, 0, 0, 255});
    sceneRender(world);

    rendererSaveBuffer(context.renderer, "screenshot.bmp");
    rendererSwapBuffer(context.renderer);
    while (context.running)
    {
        pollEvents();
#ifndef __linux__
        rendererSwapBuffer(renderer);
#endif
    }

    freeSceneAndModels(world);
}

