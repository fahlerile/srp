# rasterizer

A rasterizer in C. Is able to draw shaded triangles in screen-space for now.

## Building

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
- [x] Triangle color shading
- [ ] Triangle rasterization convention (no overlapping)
- [ ] Polygon test
- [ ] Polygon rasterization convention (no overlapping)
- [ ] Textures
- [ ] Lighting
- [ ] Shadows
- [ ] Transformation matrices

## Issues
- SEGV if `-march=native` compiler flag is set (NULL dereferencing in the assembly of `mixColorsBaryCoordPolygon`)
