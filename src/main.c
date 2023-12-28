#include "Renderer.h"
#include "Scene.h"
#include "Model.h"
#include "draw.h"
#include "Context.h"
#include "fileUtils/fileUtils.h"

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
        -2, 2,  // x
        -2, 2,  // y
         0, 5   // z
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

void bresenhamTest()
{
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {512, 256}
    );

    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {512, 128}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {512, 0}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {256+128, 0}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {256, 0}
    );

    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {512, 256+128}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {512, 512}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {512-128, 512}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {256, 512}
    );

    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {0, 128}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {0, 0}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {256-128, 0}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {0, 256}
    );

    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {0, 256+128}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {0, 512}
    );
    drawLineBresenham(
        (Vector2i) {256, 256},
        (Vector2i) {128, 512}
    );

}

