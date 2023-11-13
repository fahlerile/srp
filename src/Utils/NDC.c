#include "NDC.h"
#include "Renderer/Renderer.h"

Vector3d NDCtoScreenSpace(Renderer* renderer, Vector3d NDC)
{
    Vector2i dimensions = getWindowDimensions(renderer);
    return (Vector3d) {
        NDC.x / dimensions.x * 2 - 1,
        NDC.y / dimensions.y * (-2) + 1,
        NDC.z
    };
}
