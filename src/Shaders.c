#include "memoryUtils/memoryUtils.h"
#include "Shaders.h"
#include "Context.h"
#include "log.h"

void* vertexShaderOutputGetAttributePointerByIndex(void* vsOutput, size_t index)
{
    return indexVoidPointer(
        vsOutput, context.vertexShaderOutputInformation.attributeOffsets[index], 1
    );
}

void interpolateVertexShaderOutputInTriangle(
    void* threeVSOutput, Vector3d barycentricCoordinates, void* interpolatedVSOutput
)
{
    // interpolated_elem = SUM(elem_i * bary_i) for i [1..3]
    for (
        size_t attribute_i = 0;
        attribute_i < context.vertexShaderOutputInformation.nAttributes;
        attribute_i++
    )
    {
        switch (context.vertexShaderOutputInformation.attributeTypes[attribute_i])
        {
        case TypeVector3d:
            Vector3d interpolated = {0};
            for (size_t vertex_i = 0; vertex_i < 3; vertex_i++)
            {
                Vector3d attribute = \
                    *(Vector3d*) vertexShaderOutputGetAttributePointerByIndex(
                        indexVoidPointer(
                            threeVSOutput, vertex_i,
                            context.vertexShaderOutputInformation.nBytes
                        ),
                        attribute_i
                    );
                Vector3d weighted = Vector3dMultiplyD(
                    attribute, Vector3dIndex(barycentricCoordinates, vertex_i)
                );
                interpolated = Vector3dAdd(interpolated, weighted);
            }

            void* destination = vertexShaderOutputGetAttributePointerByIndex(
                interpolatedVSOutput, attribute_i
            );
            memcpy(destination, &interpolated, sizeof(Vector3d));
        break;
        case TypeVector4d:
            LOGE("interpolateVertexShaderOutputInTriangle: NOT IMPLEMENTED!\n");
        break;
        default:
            LOGE("interpolateVertexShaderOutputInTriangle: Invalid attribute type!\n");
        break;
        }
    }
}

void vertexShaderLoadAttributesFromVertexPointer(
    void* p_vertex, VertexBuffer* vertexBuffer, ...
)
{
    va_list args;
    va_start(args, vertexBuffer);

    for (size_t i = 0; i < context.vertexShaderOutputInformation.nAttributes; i++)
    {
        void* p_dest = va_arg(args, void*);
        size_t nBytes = va_arg(args, size_t);

        memcpy(p_dest, VertexPointerGetAttributePointerByIndex(
            vertexBuffer, p_vertex, i
        ), nBytes);
    }

    va_end(args);
}

void vertexShaderCopyToOutputBuffer(void* outputBuffer, ...)
{
    va_list args;
    va_start(args, outputBuffer);

    for (size_t i = 0; i < context.vertexShaderOutputInformation.nAttributes; i++)
    {
        void* p_data = va_arg(args, void*);
        size_t nBytes = va_arg(args, size_t);

        memcpy(indexVoidPointer(
            outputBuffer, 
            context.vertexShaderOutputInformation.attributeOffsets[i], 1
        ), p_data, nBytes);
    }

    va_end(args);
}

