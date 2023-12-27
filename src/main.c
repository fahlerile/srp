#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "Context.h"

Context context;

int main(int argc, char** argv)
{
    constructContext(&context);

    Matrix4 viewMatrix = Matrix4ConstructView(
        (Vector3d) {0, 0, 0},  // translate
        (Vector3d) {0, 0, 0},  // rotate
        (Vector3d) {1, 1, 1}   // scale
    );  
    Matrix4 projectionMatrix = Matrix4ConstructOrthogonalProjection(
        -2, 2,  // left, right
        -2, 2,  // bottom, top
         1, 5   // near, far
    );
    Scene* world = newScene(viewMatrix, projectionMatrix);
    Model* teapot = newModel("res/models/cube.obj");
    modelAddInstance(teapot, 
        (Vector3d) {0, 0, 3},
        (Vector3d) {0, 0, 0},
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

