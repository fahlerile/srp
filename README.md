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
- [x] Think if my algorithms work for both clockwise and anticlockwise vertices' direction FOR TRIANGLES
    - Yes, everything should work fine for any vertices' direction IN A TRIANGLE
- [x] Fix a precision bug (set a tolerance compare for equality of sum of barycentric coordinates and 1)
- [ ] Test for different triangles (both clockwise and anticlockwise)
- [ ] Triangle rasterization convention (no overlapping)

- [ ] Polygon test
- [x] Think if my algorithms work for both clockwise and anticlockwise vertices' direction FOR CONVEX POLYGONS
- [ ] Polygon rasterization convention (no overlapping)

- [ ] Textures
- [ ] Lighting
- [ ] Shadows
- [ ] Transformation matrices

## Issues
- SEGV if `-march=native` compiler flag is set (NULL dereferencing in the assembly of `mixColorsBaryCoordPolygon`)
