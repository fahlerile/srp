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
    - [ ] Remove `Face` structure, create `Triangle` typedef for `Vertex[3]`
    - [ ] Avoid heap allocation in `modelRender`
    - [ ] Make `modelRender` to draw ONLY ONE instance of a model (remove `matrices` field in `Model` structure)
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
