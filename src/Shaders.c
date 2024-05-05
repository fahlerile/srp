#include "Shaders.h"

void shaderProgramSetDefaultGeometryShader(ShaderProgram* sp)
{
    GeometryShader* gs = &sp->geometryShader;
    gs->shader = NULL;
    gs->nBytesPerOutputVertex = sp->vertexShader.nBytesPerOutputVertex;
    gs->nOutputAttributes = sp->vertexShader.nOutputAttributes;
    // points to the same buffer!
    gs->outputAttributes = sp->vertexShader.outputAttributes;
    gs->indexOfOutputPositionAttribute = sp->vertexShader.indexOfOutputPositionAttribute;
    gs->nOutputVertices = 0;
    gs->inputPrimitive = PRIMITIVE_ANY;
    gs->outputPrimitive = PRIMITIVE_ANY;
}

