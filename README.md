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
- [x] Triangle color shading
- [x] Think if my algorithms work for both clockwise and anticlockwise vertices' direction FOR TRIANGLES
    - Yes, everything should work fine for any vertices' direction IN A TRIANGLE
- [x] Fix a precision bug (set a tolerance compare for equality of sum of barycentric coordinates and 1)
- [x] Test for different triangles (both clockwise and anticlockwise)
- [x] Add triangle rasterization rules ([top-left](https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-rasterizer-stage-rules))
- [x] Test triangle rasterization rules
    - [x] Left/right edges touch
    - [x] Top/botom edges touch
- [x] Incremental computation
- [x] Refactor/clean
- [x] Textures
- [ ] Think of a way to structure world space meshes
    - [ ] I need a neat way to use dynamic-sized array, that is, some "`std::vector` alternative"
- [ ] Vertex data parser (.obj?)
- [x] `Scene` struct
- [x] `Model` struct
    - [x] Fix memory leak with `Model` freeing
- [x] Matrices
- [ ] Perspective projection
- [ ] Depth buffer
- [ ] Lighting
- [ ] Shadows

## Issues
- SEGV if `-march=native` compiler flag is set (NULL dereferencing in the assembly of `mixColorsBaryCoordPolygon`)
