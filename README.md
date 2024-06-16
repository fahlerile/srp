# rasterizer

A rasterizer in C that uses right-hand coordinate system ([just like in OpenGL](https://learnopengl.com/Getting-started/Coordinate-Systems): +X on the right, +Y on the top and +Z behind the camera).

## Building

Prerequsites:
- SDL2 installed in C compiler path

```bash
git clone https://www.github.com/fahlerile/rasterizer --recursive
cd rasterizer
mkdir build
cd build
cmake ..
make
cd bin
./rasterizer
```

## TODO
### Documentation/examples
- [ ] Prettify the API (think about convenience macros/functions)
- [ ] Add comments to the examples
- [ ] Write the documentation about the main pipeline and individual functions

### Features
- [ ] Add interpolation with perspective correction
- [ ] Fix rasterization rules (see the gaps between triangles in `03_spinning_textured_cube` example)
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)
- [ ] Add multisampling
- [ ] Advanced texture techniques:
    - [ ] (Bi)linear filtering
    - [ ] Mipmapping
    - [ ] Anisotropic filtering
    - [ ] Transparent textures?

### Optimization
- [ ] Use a profiler to find bottlenecks in frequently-used functions
- [ ] Draw primitives in parallel (OpenMP)

### Portability

