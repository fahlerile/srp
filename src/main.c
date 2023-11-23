#include <stdio.h>
#include "Utils/Utils.h"
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "MainUtils/MainUtils.h"

int main(int argc, char** argv)
{
    Renderer* renderer = initialize(256, 256, 0);
    SDL_Event event;
    bool running = true;

    Matrix4 viewMatrix = Matrix4ConstructView((Vector3d) {0, 0, 0},
                                              (Vector3d) {0, 0, 0},
                                              (Vector3d) {0, 0, 0});
    Scene* world = newScene(viewMatrix);
    Model* teapot = newModel("res/models/utah_teapot.obj");
    modelAddInstance(teapot, (Vector3d) {0, 0, 0}, (Vector3d) {0, 0, 0}, (Vector3d) {0, 0, 0});
    sceneAddModel(world, teapot);
    freeSceneAndModels(world);

    // Draw once & save - then loop forever
    rendererClearBuffer(renderer, (Color) {0, 0, 0, 255});
    rendererDrawToBuffer(renderer);
    rendererSaveBuffer(renderer, "screenshot.bmp");
    rendererSwapBuffer(renderer);
    while (running)
    {
        pollEvents(&running, &event);
        rendererSwapBuffer(renderer);
    }

    deinitialize(renderer);
}
