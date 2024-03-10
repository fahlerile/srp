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

