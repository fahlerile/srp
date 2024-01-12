#include <stdio.h>
#include <string.h>
#include "DynamicArray/DynamicArray.h"
#include "fileUtils/fileUtils.h"
#include "Model.h"
#include "Triangle.h"
#include "Line.h"
#include "Context.h"

Model* newModel(const char* filename)
{
    Model* this = xmalloc(sizeof(Model));

    this->vertexPositions = newDynamicArray(100, sizeof(Vector4d), NULL);
    this->textureCoords = newDynamicArray(100, sizeof(Vector3d), NULL);
    this->normals = newDynamicArray(100, sizeof(Vector3d), NULL);
    this->matrices = newDynamicArray(1, sizeof(Matrix4), NULL);
    this->triangles = newDynamicArray(100, sizeof(Triangle), NULL);

    modelParseObj(this, filename);

    return this;
}

static void modelParseObj(Model* this, const char* filename)
{
    FILE* fp = fopen(filename, "r");
    assert(fp != NULL);
    char* line = NULL;
    size_t length = 0;
    char lineType[3];

    while (readLine(&line, &length, fp) != EOF)
    {
        lineType[0] = '\0';  // to avoid data doubling on empty lines
        sscanf(line, "%2s", lineType);

        if (lineType[0] == '\0' || strcmp(lineType, "#") == 0)
            continue;
        else if (strcmp(lineType, "v") == 0)
        {
            double x = NAN, y = NAN, z = NAN, w = NAN;
            sscanf(line, "%s %lf %lf %lf %lf", lineType, &x, &y, &z, &w);
            if (isnan(x) || isnan(y) || isnan(z))
                continue;
            else if (isnan(w))
                w = 1;
            Vector4d position = {x, y, z, w};
            addToDynamicArray(this->vertexPositions, &position);
        }
        else if (strcmp(lineType, "vt") == 0)
        {
            double u = NAN, v = NAN, w = NAN;
            sscanf(line, "%s %lf %lf %lf", lineType, &u, &v, &w);
            if (isnan(u) || isnan(v))
                continue;
            Vector3d UVW = {u, v, w};
            addToDynamicArray(this->textureCoords, &UVW);
        }
        else if (strcmp(lineType, "vn") == 0)
        {
            double x = NAN, y = NAN, z = NAN;
            sscanf(line, "%s %lf %lf %lf", lineType, &x, &y, &z);
            if (isnan(x) || isnan(y) || isnan(z))
                continue;
            Vector3d normal = Vector3dNormalize((Vector3d) {x, y, z});
            addToDynamicArray(this->normals, &normal);
        }
        // else if (strcmp(lineType, "vp"))
        else if (strcmp(lineType, "f") == 0)
        {
            char* vertexString = NULL;
            strtok(line, " ");

            // for every vertex in a current face
            while ((vertexString = strtok(NULL, " ")) != NULL)
            {
                size_t v = 0, vt = 0, vn = 0;
                sscanf(vertexString, "%zu/%zu/%zu", &v, &vt, &vn);

                if (v == 0)  // nothing found for vertex position
                {
                    fail = true;
                    break;
                }
                
                Vector4d position = *(Vector4d*) indexDynamicArray(this->vertexPositions, v-1);
                Vector3d textureCoords = *(Vector3d*) indexDynamicArray(this->textureCoords, vt-1);
                Vector3d normal = *(Vector3d*) indexDynamicArray(this->normals, vn-1);

                VertexOBJ vertex = {
                    .position = position,
                    .textureCoords = textureCoords,
                    .normal = normal
                };

                addToDynamicArray(faceVertices, vertex);
            }

            if (fail)
            {
                deleteAllFromDynamicArray(faceVertices);
                continue;
            }

            if (faceVertices->size == 3)
            {
                // Load vertices into VBO if there will be no duplicates
                // Add EBO indices
            }
            else
            {
                DynamicArray* triangles = triangulateFace(faceVertices);
                // Get unique vertices from array of `Triangle`'s, load them into VBO
                // Add EBO indices
                VBOAddDynamicArrayOfVertices();
            }
        }
        // else if (strcmp(lineType, "g"))
        // else if (strcmp(lineType, "o"))
        else
            LOGE("Unknown line type \"%s\" occured during parsing of %s\n", lineType, filename);
    }

    xfree(line);
    freeDynamicArray(faceVertices);
    fclose(fp);
}

void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale)
{
    Matrix4 mat = Matrix4ConstructTRS(position, rotation, scale);
    addToDynamicArray(this->matrices, &mat);
}

void modelRender(Model* this, Matrix4* view, Matrix4* projection)
{
    for (size_t matrixI = 0; matrixI < this->matrices->size; matrixI++)
    {
        Matrix4* modelMatrix = indexDynamicArray(this->matrices, matrixI);
        Matrix4 MV = Matrix4MultiplyMatrix4(view, modelMatrix);
        Matrix4 MVP = Matrix4MultiplyMatrix4(projection, &MV);
        
        for (size_t faceI = 0; faceI < this->triangles->size; faceI++)
        {
            Triangle* triangle = indexDynamicArray(this->triangles, faceI);
            Vertex verticesTransformed[3];
            
            Vector4d verticesPositions[3];
            Vector3d verticesTextureCoords[3];
            Vector3d verticesNormals[3];

            for (size_t vertexI = 0; vertexI < 3; vertexI++)
            {
                Vertex* vertex = &(triangle->vertices[vertexI]);

                verticesPositions[vertexI] = Matrix4MultiplyVector4dHomogeneous(
                    &MVP, *(vertex->position)
                );
                verticesTextureCoords[vertexI] = *(vertex->UV);
                verticesNormals[vertexI] = *(vertex->normal);

                verticesTransformed[vertexI] = (Vertex) {
                    .position = &(verticesPositions[vertexI]),
                    .UV =       &(verticesTextureCoords[vertexI]),
                    .normal =   &(verticesNormals[vertexI])
                };
            }

            Triangle triangleTransformed = {{
                verticesTransformed[0],
                verticesTransformed[1],
                verticesTransformed[2]
            }};

            // if at least one vertex of a face is inside of unit cube
            if (!areAllVerticesOfATriangleOutsideOfUnitCube(&triangleTransformed))
                drawTriangle(&triangleTransformed);
        }
    }
}

void freeModel(Model* this)
{
    freeDynamicArray(this->vertexPositions);
    freeDynamicArray(this->textureCoords);
    freeDynamicArray(this->normals);
    freeDynamicArray(this->matrices);
    freeDynamicArray(this->triangles);
    xfree(this);
}
