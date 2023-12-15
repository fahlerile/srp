#pragma once
#include "Vectors/Vector3.h"
#include "Renderer.h"

// Converts NDC point to screen space. Z coordinate is left as is, W is dropped
Vector3d NDCtoScreenSpace(Renderer* renderer, Vector4d NDC);

