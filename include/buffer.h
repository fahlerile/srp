// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

/** @file
 *  Buffer objects (SRPVertexBuffer and SRPIndexBuffer) */

#pragma once

#include "framebuffer.h"
#include "shaders.h"

/** @ingroup Buffer
 *  @{ */

/** Specifies primitives that can be drawn
 *  @see srpDrawVertexBuffer() srpDrawIndexBuffer() */
typedef enum SRPPrimitive
{
	SRP_PRIM_POINTS,          /**< Draw points (0, 1, 2, 3, ..., n-1) */
	SRP_PRIM_LINES,           /**< Draw lines (0-1, 2-3, ...) If non-even N of vertices,
	                               then the extra one is ignored) */
	SRP_PRIM_LINE_STRIP,      /**< Draw lines (0-1, 1-2, ..., {n-2}-{n-1}) */
	SRP_PRIM_LINE_LOOP,       /**< Draw lines (0-1, 1-2, ..., {n-2}-{n-1}, {n-1}-0) */
	SRP_PRIM_TRIANGLES,       /**< Draw triangles (0-1-2, 3-4-5, ...) */
	SRP_PRIM_TRIANGLE_STRIP,  /**< Draw triangles (0-1-2, 1-2-3, 2-3-4, ...) */
	SRP_PRIM_TRIANGLE_FAN     /**< Draw triangles (0-1-2, 0-2-3, 0-3-4, ...) */
} SRPPrimitive;

/** Stores vertex data, similarly to VBO in OpenGL */
typedef struct SRPVertexBuffer
{
	size_t nBytesPerVertex;
	size_t nBytesData;
	size_t nVertices;
	SRPVertex* data;
} SRPVertexBuffer;

/** Stores indices to vertices from SRPVertexBuffer,
 *  similarly to EBO in OpenGL */
typedef struct SRPIndexBuffer
{
	SRPType indicesType;
	size_t nBytesPerIndex;
	size_t nIndices;
	void* data;
} SRPIndexBuffer;

/** Initialize the vertex buffer with vertex data
 *  @param[in] nBytesPerVertex The size of one vertex, in bytes
 *  @param[in] nBytesData The size of vertex data, in bytes
 *  @param[in] data The pointer to vertex data
 *  @return The pointer to constructed vertex buffer */
SRPVertexBuffer* srpNewVertexBuffer
	(size_t nBytesPerVertex, size_t nBytesData, const void* data);

/** Free the vertex buffer
 *  @param[in] this The pointer to vertex buffer, as returned from srpNewVertexBuffer() */
void srpFreeVertexBuffer(SRPVertexBuffer* this);

/** Draw vertices from vertex buffer
 *  @param[in] this The vertex buffer to read the vertex data from
 *  @param[in] fb The framebuffer to draw to
 *  @param[in] sp The shader program to use
 *  @param[in] primitive Specifies the primitive to draw
 *  @param[in] startIndex Specifies from what index to start drawing
 *  @param[in] count Specifies how many vertices to draw */
void srpDrawVertexBuffer(
	const SRPVertexBuffer* this, const SRPFramebuffer* fb, const SRPShaderProgram* sp,
	SRPPrimitive primitive, size_t startIndex, size_t count
);

/** Initialize the index buffer with index data
 *  @param[in] indicesType The type of indices passed by data
 *  Must be one of TYPE_UINT8, TYPE_UINT16, TYPE_UINT32, TYPE_UIN64
 *  @param[in] nBytesData The size of index data 
 *  @param[in] data The pointer to an array of indices of type indicesType
 *  @return The pointer to constructed index buffer */
SRPIndexBuffer* srpNewIndexBuffer
	(SRPType indicesType, size_t nBytesData, const void* data);

/** Free the index buffer
 *  @param[in] this The pointer to index buffer, as returned from srpNewIndexBuffer() */
void srpFreeIndexBuffer(SRPIndexBuffer* this);

/** Draw vertices using both vertex and index buffers
 *  @param[in] this The index buffer to read the indices from
 *  @param[in] vb The vertex buffer to read the vertex data from
 *  @param[in] fb The framebuffer to draw to
 *  @param[in] sp The shader program to use
 *  @param[in] primitive Specifies the primitive to draw
 *  @param[in] startIndex Specifies from what index buffer's index to start drawing
 *  @param[in] count Specifies how many indices to draw */
void srpDrawIndexBuffer(
	const SRPIndexBuffer* this, const SRPVertexBuffer* vb, const SRPFramebuffer* fb,
	const SRPShaderProgram* sp, SRPPrimitive primitive, size_t startIndex, size_t count
);

/** @} */  // ingroup Buffer

