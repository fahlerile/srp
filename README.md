# rasterizer

A rasterizer in C. Is able to draw shaded or textured triangles in screen-space for now.

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
- [x] `copyDynamicArray`
- [ ] `triangulateFace` 
- [ ] Projection matrices
    - [x] Orthogonal projection matrix construction
    - [x] Vertices transformation in `modelRender`
    - [x] Add draw calls in `modelRender`
        - [x] Are draw calls overly complex? Do I need a `Triangle` structure? Maybe I want to rather have `renderFace` function?
            - Yes, they are unnecessarily complex. I do not need to have a `Triangle` structure, instead I am going to create a `drawFace` function which will triangulate a face and draw it using the already written code (the one in `src/Triangle.c`)
        - [x] Delete `Triangle` struct
        - [x] Rename `Triangle.c` to `Face.c`
        - [x] Move `Face` struct delaration to `Face.c`
        - [x] Restructure the code to form the `drawFace` function
    - [x] Fix memory bug (`copyFace`)
    - [ ] Perspective projection matrix construction
- [ ] Line drawing function (for `faceModeLine`)
- [ ] Refactor & optimize
    - [x] Move utilities that I may use in other projects to own git repos
        - [x] Restructure build system
        - [x] Fix linking to SDL2 for windows (build from source..?)
        - [x] Use git submodules
    - [x] Delete `mainUtils`, add global context
    - [x] Fix not calling `SDL_Quit` and `SDL_MainReady` (`atexit`)
    - [ ] Is the structure overly complex?
    - [ ] Can I avoid copying whole faces in `modelRender`?
- [ ] DOCUMENTATION!
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
