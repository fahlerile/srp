#pragma once
#include "Renderer.h"

// Convert NDC coordinates to Screen space coordinates
// Leaves `z` the same
inline static Vector3d NDCToScreenSpace(Renderer* renderer, Vector3d NDC)
{
    Vector2i dimensions = renderer->dimensions;
    return (Vector3d) {
         ((double) dimensions.x / 2) * (NDC.x + 1) + 0.5,
        -((double) dimensions.y / 2) * (NDC.y - 1) + 0.5,
        NDC.z
    };
}

