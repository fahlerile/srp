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
- [ ] `triangulateFace`
- [x] Projection matrices
- [x] Test wireframe rendering on various models
- [ ] Refactor & optimize
    - [ ] Is the structure overly complex?
    - [ ] Can I avoid copying whole faces in `modelRender`?
- [ ] DOCUMENTATION!
- [ ] Color interpolation
- [ ] Textures, UV-mapping
- [ ] Depth buffer
- [ ] Lighting
- [ ] Shadows
- [ ] Integration testing
    - Check whether or not drawn triangle/polygon/model correctly (image comparison?)

## Issues
- SEGV if `-march=native` compiler flag is set
