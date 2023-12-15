#include <stdio.h>
#include "utils/utils.h"
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
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
        (Vector3d) {0, 0, 0},
        (Vector3d) {0, 0, 0},
        (Vector3d) {0, 0, 0}
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
        (Vector3d) {0, 0, 0}
    );
    sceneAddModel(world, teapot);
    
    sceneRender(world, renderer);

    freeSceneAndModels(world);

    // Draw once & save - then loop forever
    rendererClearBuffer(renderer, (Color) {0, 0, 0, 255});
    rendererSaveBuffer(renderer, "screenshot.bmp");
    rendererSwapBuffer(renderer);
    while (running)
    {
        pollEvents(&running, &event);
#ifndef __linux__
        rendererSwapBuffer(renderer);
#endif
    }

    deinitialize(renderer);
}
