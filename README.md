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
- [x] Write some examples
- [ ] Add comments to the examples
- [ ] Prettify the API (think about convenience macros/functions)
- [ ] Write the documentation about the main pipeline and individual functions

### Features
- [x] Change shader signatures to use new input and output structures; add some builtin shader inputs from OpenGL
- [ ] Add texture mapping
- [ ] Add interpolation with perspective correction
- [ ] Fix rasterization rules
- [ ] Consider the case of multiple `Vertex` types, and changes between them during the rendering pipeline (see \`TODO\` comments)
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)

### Optimization
- [ ] Draw primitives in parallel (OpenMP)

### Portability
- [x] Do not use `lib/c-codebase` types for API function signatures for portability and avoiding useless dependency
- [x] Fix header hell
- [ ] Split everything into public and private headers
- [ ] Remove `lib/c-codebase` dependency

