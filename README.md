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
- [ ] Change vertices' winding order to counter-clockwise to be consistent with OpenGL (right now `drawTriangle` only draws clockwise triangles, not the counter-clockwise ones)
- [ ] Implement back-face culling
- [ ] Add `Uniforms`
- [ ] Add texture mapping with perspective correction
- [ ] Think about making an API more user-friendly
- [ ] Add `Mesh`/`Model` structure
- [ ] Play around with rendering models
- [ ] Draw primitives in parallel (OpenMP)
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)
- [ ] Write implementation-level documentation
- [ ] Prettify the API, write the user-level documentation
- [ ] Write some examples

- [ ] Read LearnOpenGL to learn about light, reflections, shadows and all that fancy stuff and think how can I implement that

