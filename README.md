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
- [x] Add `IndexBuffer`
	- [x] Fix unreasonable `size_t` usage in `drawIndexBuffer`
- [x] Change vertices' winding order to counter-clockwise to be consistent with OpenGL (right now `drawTriangle` only draws clockwise triangles, not the counter-clockwise ones)
- [x] Implement back-face culling in screen space (do not draw triangles with CW winding)
- [x] Add `Uniforms` with named parameters
- [x] Write some examples

- [ ] Review the rendering pipeline (https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview)
- [x] Do not use `lib/c-codebase` types for API function signatures
- [ ] Prettify the API, write the documentation

- [ ] Add texture mapping with perspective correction

- [ ] Add `Mesh`/`Model` structure
- [ ] Play around with rendering models

- [ ] Draw primitives in parallel (OpenMP)
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)

- [ ] Read LearnOpenGL to learn about light, reflections, shadows and all that fancy stuff and think how can I implement that

