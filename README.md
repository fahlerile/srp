# rasterizer

A rasterizer in C. Is able to draw shaded or textured triangles in screen-space for now.

## Building

```bash
git clone ...
cd rasterizer
mkdir build
cd build
cmake ..
make
cd bin
./rasterizer
```

## TODO
Rendering models:
- [x] Think of a way to structure world space models
    - [x] I need a neat way to use dynamic-sized array, that is, some "`std::vector` alternative"
        - `DynamicArray` struct
- [x] Matrices
- [x] `Scene` struct
- [x] `Model` struct
    - [x] Fix memory leak with `Model` freeing
- [ ] Vertex, face, edge structures (if I need them)
- [ ] Vertex data parser in `Model`
- [ ] Perspective projection
- [ ] Depth buffer
    - [ ] Interpolate z-coordinate inside triangle, draw a pixel only if that z coordinate is closer to viewport than already drawn one
- [ ] Lighting
- [ ] Shadows

Documentation/testing:
- [ ] Start documenting the code
    - [ ] Triangle (computation, algorithm, etc.)
    - [ ] DynamicArray (why & how it works)
- [ ] Unit testing (for *some* utilities)
    - [ ] Matrix
    - [ ] Vectors
    - [x] DynamicArray
- [ ] Integration testing
    - Check whether or not drawn triangle/polygon/model correctly (image comparison?)

Refactoring:
- Triangle:
    - [x] Change barycentric coordinates' type to just array, not `Vector3d`
        - No, this is not a good idea
    - [x] Split up `triangleDraw` into multiple functions
        - No ideas how to do this
    - [x] Split up `newTriangle` into multiple functions
        - I might need them if I decide to add "editing" functionality (construct a `Triangle` once - use a million times by changing vertices/UV/texture)
- Matrix:
    - [ ] Pass-by-pointer VS pass-by-value

## Issues
- SEGV if `-march=native` compiler flag is set (NULL dereferencing in the assembly of `mixColorsBaryCoordPolygon`)
    - (may be outdated)
