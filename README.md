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
- [x] Rewrite tile-based rasterizer and shader utilities
- [x] Add fragment shader call and interpolation in `triangleLoopOverTileAndFill`
- [x] Replace the 100500 `if (sp->geometryShader.shader == NULL)` checks with default geometry shader if it is not initialized by the user
- [x] Add more "sanity checks" to avoid the user to shoot themselves in the foot
    - Pass `ShaderProgram*` to shaders to avoid repetition
- [x] Test vertex, geometry and fragment shaders
- [x] Implement Z-buffer
- [ ] Investigate #8
- [ ] Optimize the rasterizer
    - Ideas:
        - Assembly-level optimizations
        - Play around with `tileDimensions` computation (`calculateTileDimensionsAndNTilesInBoundingBox`)
        - Draw multiple primitives at the same tile (z-buffer issue)
        - Draw multiple tiles at the same time
- [ ] Write implementation-level documentation
- [ ] Add `indexBuffer`
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)
- [ ] Extensively test shader with new primitives and using `indexBuffer`
- [ ] Add convenience macros/functions to avoid giving the user an ability to ~~shoot themselves in the foot~~ blow their whole leg off
- [ ] Prettify the API, write the use-level documentation
- [ ] Write some examples

