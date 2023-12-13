# rasterizer

A rasterizer in C. Is able to draw shaded or textured triangles in screen-space for now.

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
### Rendering models:
- [x] Think of a way to structure world space models
    - [x] I need a neat way to use dynamic-sized array, that is, some "`std::vector` alternative"
        - `DynamicArray` struct
- [x] Matrices
- [x] `Scene` struct
- [x] `Model` struct
    - [x] Fix memory leak with `Model` freeing
- [x] Face parser in `Model`
    - [x] Determine whether or not `readLine` reads INCLUDING '\n' or excluding and document it!
    - [ ] `splitString` (safe strtok implementation, I do not want to use GNU `strtok_r`)
        - [x] ~~ Generic `LinkedList` implementation ~~
        - [ ] ~~ Unit test for `LinkedList` ~~
        - [x] Add freeing for `splitString` (add free callback to `DynamicArray`)
            - [x] Add a test for this callback
            - [x] Make everything use this callback
    - [x] Vertex, face structures
- [ ] Projection matrices
    - [ ] Orthogonal projection matrix construction
    - [ ] Perspective projection matrix construction
    - [ ] MVP matrix transformation in `modelRender`
    - [ ] Draw calls in `modelRender`
- [ ] Depth buffer
    - [ ] Interpolate z-coordinate inside triangle, draw a pixel only if that z coordinate is closer to viewport than already drawn one
- [ ] Lighting
- [ ] Shadows

### Documentation/testing:
- [x] Start documenting the code
    - [x] Triangle (computation, algorithm, etc.)
    - [x] ~~DynamicArray (why & how it works)~~
- [x] Unit testing (for *some* utilities)
    - [x] Matrix
    - [x] Vectors
    - [x] DynamicArray
- [ ] Integration testing
    - Check whether or not drawn triangle/polygon/model correctly (image comparison?)

### Refactoring:
- Triangle:
    - [x] ~~Change barycentric coordinates' type to just array, not `Vector3d`~~
    - [x] ~~Split up `triangleDraw` into multiple functions~~
    - [x] Split up `newTriangle` into multiple functions
        - I might need them if I decide to add "editing" functionality (construct a `Triangle` once - use a million times by changing vertices/UV/texture)
- Matrix:
    - [x] Pass-by-pointer VS pass-by-value
        - Pass by pointer

## Issues
- SEGV if `-march=native` compiler flag is set
