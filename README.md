# rasterizer

A rasterizer in C that uses right-hand coordinate system (camera looks in +Z).

## Building

Prerequsites:
- SDL2 installed in C compiler path

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
- [x] `triangulateFace` (NOT TESTED!)
- [x] Projection matrices
- [x] Test wireframe rendering on various models
- [ ] Refactor & optimize
    - [x] Remove `Face` structure, create `Triangle` typedef for `Vertex[3]`
    - [x] Rewrite `.obj` parser for `f` line type
    - [x] Avoid heap allocation in `modelRender`
    - [ ] Make `modelRender` to draw ONLY ONE instance of a model (remove `matrices` field in `Model` structure)
    - [ ] Think about "shader support" (a function that gets called in `modelRender`? Then how to implement "uniforms"?)
    - [ ] Think about "`Vertex` variability" (e.g. having color data instead of UV, adding additional data to vertices and etc. like in OpenGL)
    - [ ] Optimize Bresenham's line drawing algorithm implementation
- [ ] DOCUMENTATION!
- [ ] Depth buffer
- [ ] Shaders
    - [ ] Color interpolation
    - [ ] Textures, UV-mapping
- [ ] Lighting
- [ ] Shadows
- [ ] Testing
    - Check whether or not drawn triangle/polygon/model correctly (image comparison?)

## Issues
- SEGV if `-march=native` compiler flag is set
