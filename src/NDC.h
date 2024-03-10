#pragma once
#include "Renderer.h"

// Convert NDC coordinates to Screen space coordinates
// Leaves `z` the same
inline static Vector3d NDCToScreenSpace(Renderer* renderer, Vector3d NDC)
{
    return (Vector3d) {
         (((double) renderer->dimensions.x - 1) / 2) * (NDC.x + 1) + 0.5,
        -(((double) renderer->dimensions.y - 1) / 2) * (NDC.y - 1) + 0.5,
        NDC.z
    };
}

