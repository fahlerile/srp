# rasterizer

A rasterizer in C that uses right-hand coordinate system (camera looks in +Z).

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
- [ ] Add fragment shader call and interpolation in `triangleLoopOverTileAndFill`
- [ ] Test vertex, geometry and fragment shaders
- [ ] Implement z-buffer
- [ ] Optimize the rasterizer
    - Ideas:
        - Play around with `tileDimensions` computation (`calculateTileDimensionsAndNTilesInBoundingBox`)
        - Draw multiple primitives at the same tile (z-buffer issue)
        - Draw multiple tiles at the same time
        - Assembly-level optimizations
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)
- [ ] Test geometry shader for new primitives

