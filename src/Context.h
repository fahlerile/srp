#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Renderer.h"
#include "Uniforms.h"
#include "Shaders.h"

typedef enum
{
    drawingModeFill = 0,
    drawingModeLine
} drawingMode;

typedef struct
{
    bool running;
    Renderer* renderer;
    SDL_Event event;
    
    Uniforms* uniforms;
    drawingMode drawingMode;

    VertexShaderType vertexShader;
    VertexShaderOutputInformation vertexShaderOutputInformation;
    FragmentShaderType fragmentShader;
} Context;

extern Context context;

void constructContext(Context* this);
void destroyContext();

void pollEvents();

