#include <stdio.h>
#include <string.h>
#include "Model.h"
#include "Face.h"
#include "utils/utils.h"
#include "fileUtils/fileUtils.h"
#include "stringUtils/stringUtils.h"

static void faceFreeCallback(void* p_face)
{
    freeFace((Face*) p_face);
}

Model* newModel(const char* filename)
{
    Model* this = xmalloc(sizeof(Model));

    this->vertexPositions = newDynamicArray(100, sizeof(Vector4d), NULL);
    this->UVs = newDynamicArray(100, sizeof(Vector2d), NULL);
    this->normals = newDynamicArray(100, sizeof(Vector3d), NULL);
    this->matrices = newDynamicArray(10, sizeof(Matrix4), NULL);
    this->faces = newDynamicArray(100, sizeof(Face*), faceFreeCallback);

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
            Vector2d UV = {u, v};
            addToDynamicArray(this->UVs, &UV);
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
            DynamicArray* vertices = newDynamicArray(3, sizeof(Vertex), NULL);
            DynamicArray* vertices_str = splitString(line, " ");  // char*
            bool fail = false;

            // starting with 1 because we do not need `f`
            for (size_t i = 1; i < vertices_str->size; i++)
            {
                char* token = *(char**) indexDynamicArray(vertices_str, i);
                size_t v = 0, vt = 0, vn = 0;
                sscanf(token, "%zu/%zu/%zu", &v, &vt, &vn);
                if (v == 0)
                {
                    fail = true;
                    break;
                }

                // -1 because OBJ is 1-base indexed
                Vertex vert = {
                    .position = indexDynamicArray(this->vertexPositions, v - 1),
                    .UV = indexDynamicArray(this->UVs, vt - 1),
                    .normal = indexDynamicArray(this->normals, vn - 1)
                };

                addToDynamicArray(vertices, &vert);
            }
            freeDynamicArray(vertices_str);
            if (fail)
            {
                freeDynamicArray(vertices);
                continue;
            }

            Face* face = newFace(vertices);
            addToDynamicArray(this->faces, &face);
        }
        // else if (strcmp(lineType, "g"))
        // else if (strcmp(lineType, "o"))
        else
            LOGE("Unknown line type \"%s\" occured during parsing of %s\n", lineType, filename);
    }
    xfree(line);
    fclose(fp);
}

void modelAddInstance(Model* this, Vector3d position, Vector3d rotation, Vector3d scale)
{
    Matrix4 mat = Matrix4ConstructTRS(position, rotation, scale);
    addToDynamicArray(this->matrices, &mat);
}

void modelRender(Model* this, Matrix4* view, Matrix4* projection, Renderer* renderer)
{
    for (size_t i = 0; i < this->matrices->size; i++)
    {
        Matrix4* model = indexDynamicArray(this->matrices, i);
        Matrix4 MV = Matrix4MultiplyMatrix4(view, model);
        Matrix4 MVP = Matrix4MultiplyMatrix4(projection, &MV);
        
        DynamicArray* transformedPositions = newDynamicArray(3, sizeof(Vector4d), NULL);
        transformedPositions->size = 3;

        for (size_t face_i = 0; face_i < this->faces->size; face_i++)
        {
            // allocating 7 petabytes here (wtf?)
            Face* copiedFace = copyFace(*(Face**) indexDynamicArray(this->faces, face_i));
            for (size_t vertex_i = 0; vertex_i < copiedFace->vertices->size; vertex_i++)
            {
                Vertex* p_curVertex = indexDynamicArray(copiedFace->vertices, vertex_i);
                Vector4d* p_curVertexPosition = p_curVertex->position;
                Vector4d transformedVertexPosition = Matrix4MultiplyVector4d(&MVP, *p_curVertexPosition);
                setInDynamicArray(transformedPositions, &transformedVertexPosition, vertex_i);
                ((Vertex*) indexDynamicArray(copiedFace->vertices, vertex_i))->position = (Vector4d*) indexDynamicArray(transformedPositions, vertex_i);
            } 

            drawFace(copiedFace, renderer);
        }
        freeDynamicArray(transformedPositions);
    }
}

void freeModel(Model* this)
{
    freeDynamicArray(this->vertexPositions);
    freeDynamicArray(this->UVs);
    freeDynamicArray(this->normals);
    freeDynamicArray(this->matrices);
    freeDynamicArray(this->faces);
    xfree(this);
}
