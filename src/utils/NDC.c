#include "NDC.h"
#include "Renderer.h"

Vector3d NDCtoScreenSpace(Renderer* renderer, Vector3d NDC)
{
    Vector2i dimensions = rendererGetWindowDimensions(renderer);
    return (Vector3d) {
         (dimensions.x / 2) * (NDC.x + 1),
        -(dimensions.y / 2) * (NDC.y - 1),
        NDC.z
    };
}
