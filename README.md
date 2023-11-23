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
- [ ] Unit testing (for utilities)
    - [ ] Matrix
    - [ ] Vectors
    - [ ] MemoryUtils
    - [ ] NDC
    - [ ] radians
    - [ ] log
    - [ ] DynamicArray
    - [ ] compares
    - [ ] Color
- [ ] Integration testing
    - ???

## Issues
- SEGV if `-march=native` compiler flag is set (NULL dereferencing in the assembly of `mixColorsBaryCoordPolygon`)
    - (may be outdated)
