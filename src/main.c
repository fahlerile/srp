#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "mainUtils/mainUtils.h"

int main(int argc, char** argv)
{
    Renderer* renderer = initialize(256, 256, 0);
    SDL_Event event;
    bool running = true;

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
    
    rendererClearBuffer(renderer, (Color) {0, 0, 0, 255});
    sceneRender(world, renderer);

    rendererSaveBuffer(renderer, "screenshot.bmp");
    rendererSwapBuffer(renderer);
    while (running)
    {
        pollEvents(&running, &event);
#ifndef __linux__
        rendererSwapBuffer(renderer);
#endif
    }

    freeSceneAndModels(world);
    deinitialize(renderer);
}
