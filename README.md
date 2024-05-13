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
- [ ] Review the rendering pipeline (https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview)
	- See if I am missing some important step
	- See if I can make the pipeline more customizable
- [ ] Add texture mapping with perspective correction
- [ ] Add `Mesh`/`Model` structure
- [ ] Play around with rendering models
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)
- [ ] Read LearnOpenGL to learn about light, reflections, shadows and all that fancy stuff and think how can I implement that

### Optimization
- [ ] Draw primitives in parallel (OpenMP)

### Portability
- [x] Do not use `lib/c-codebase` types for API function signatures for portability and avoiding useless dependency
- [ ] Fix header hell

