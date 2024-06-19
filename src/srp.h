// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

#pragma once

/** @file
 *  The only header the end user needs to include */

#include "context.h"
#include "buffer.h"
#include "texture.h"
#include "color.h"
#include "framebuffer.h"
#include "primitive.h"
#include "vertex.h"
#include "shaders.h"

#ifdef SRP_INCLUDE_VEC
	#include "vec.h"
#endif
#ifdef SRP_INCLUDE_MAT
	#include "mat.h"
#endif

