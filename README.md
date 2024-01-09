# rasterizer

A rasterizer in C that uses right-hand coordinate system (camera looks in +Z).

## Building

Prerequsites:
- SDL2 installed in C compiler path

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
- [x] `triangulateFace` (NOT TESTED!)
- [x] Projection matrices
- [x] Test wireframe rendering on various models
- [ ] Refactor & optimize
    - [x] Remove `Face` structure, create `Triangle` typedef for `Vertex[3]`
    - [x] Rewrite `.obj` parser for `f` line type
    - [x] Avoid heap allocation in `modelRender`
    - [x] Do not ignore `w` component in `modelParseObj`
        - `Model.UV` is now `Model.textureCoords`
        - `Model.textureCoords` is of type `Vector3d` (U, V, W)
    - [ ] Create VBO, EAO and VAO-like structures
        - `modelParseObj`, face parser:
            - For every vertex in the face:
                - [x] Get position, UV and normal data from 1-based `.obj` indices
                    - [ ] Handle the case where there are no UV coords or/and normals!
                - [ ] Add a vertex to a buffer
                - [ ] If a vertex is faulty (invalid index, no position data), proceed to the next line and delete everything stored in buffer
            - If everything is good:
                - [ ] Add vertices from a temporary buffer to a `Model`'s VBO if the same ones are not already there
                    - [ ] Think of a fast lookup/search data structure to store 3 values (`v`, `vt`, `vn` indices from `.obj`) as indices and an index in `VBO` for this vertex as value
                - [ ] Triangulate if needed
                - [ ] Add indices for EBO
        - `VBO`
            - A structure similar to `DynamicArray` (it will probably be used in implementation with `uint8` stored type)
            - Should store attribute data (store offsets to different attributes), should operate on bytes
        - `EBO`
            - A structure similar to (if not the same as) `DynamicArray`
    - [ ] Make `modelRender` to draw ONLY ONE instance of a model (remove `matrices` field in `Model` structure)
    - [ ] Think about "shader support" (a function that gets called in `modelRender`? Then how to implement "uniforms"?)
    - [ ] Think about "`Vertex` variability" (e.g. having color data instead of UV, adding additional data to vertices and etc. like in OpenGL)
    - [ ] Optimize Bresenham's line drawing algorithm implementation
- [ ] DOCUMENTATION!
- [ ] Depth buffer
- [ ] Shaders
    - [ ] Color interpolation
    - [ ] Textures, UV-mapping
- [ ] Lighting
- [ ] Shadows
- [ ] Testing
    - Check whether or not drawn triangle/polygon/model correctly (image comparison?)

## Issues
- SEGV if `-march=native` compiler flag is set
