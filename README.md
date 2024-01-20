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
    - [ ] Make `modelRender` to draw ONLY ONE instance of a model (remove `matrices` field in `Model` structure)
    - [ ] Optimize Bresenham's line drawing algorithm implementation
- [ ] "my own opengl"
    - [x] Think about "shader support" (a function that gets called in `modelRender`? Then how to implement "uniforms"?)
    - [x] Think about "`Vertex` variability" (e.g. having color data instead of UV, adding additional data to vertices and etc. like in OpenGL)
    - [x] Create VBO-like structure
    - [x] Get a triangle rendered with a vertex shader
    - [x] Think a little more about better possible alternative for index-based uniforms
        - A better approach (to reduce `Uniforms->offsets` overhead and etc.) for a `Uniforms` structure is an array of pointers to data with an ability to add a uniform (a pointer to data) to a specific slot
        - There will be names for uniforms, they will be defined by user in the shader (just like in OpenGL with `layout(location = X) uniform ...`)
        - For now, I will leave `Uniforms` as singleton structure stored in `Context` 
        - Hence, I will not pass the `Uniforms` structure in arguments and access them through global `Context`
    - [x] Test `Uniforms` structure
    - [x] Implement data interpolation inside the primitive
    - [x] Add macros/helper functions to not handle loading in/from vertex data manually
    - [ ] Refactor the code
    - [ ] Create EBO-like structure (`indexBuffer`)
    - [ ] Add line primitive
    - [ ] DIVIDE the OpenGL-like API implementation and other code (such as Model and Scene abstractions)
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
