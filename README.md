# srp

(stands for **s**oftware **r**endering **p**ipeline)

Highly inspired by OpenGL API and made only for learning purposes, this is my first serious project in C (still WIP at the moment) and my first project in the field of computer graphics. The only dependency is [`stb_image`](https://github.com/nothings/stb/blob/master/stb_image.h). You probably *do not* want to use this in production code!

This is a rendering pipeline written fully in software that uses only the CPU for computation. It features:
- Fully programmable vertex and fragment shaders
- A small math library with vector and matrix structures to use in shader programming
- Texture mapping
- Shader uniforms
- Affine attribute interpolation

## Building

```bash
git clone https://www.github.com/fahlerile/srp
mkdir srp/build
cd srp/build
cmake .. -D CMAKE_BUILD_TYPE=Release
make
cd bin
```

You can also build the examples by providing the `-D BUILD_EXAMPLES=1` argument to `cmake`.

## Similar/related projects
- https://github.com/rswinkle/PortableGL
- https://github.com/NotCamelCase/SoftLit
- https://github.com/nikolausrauch/software-rasterizer
- https://github.com/niepp/srpbr

And a lot more... Here I mentioned only those projects on which I looked at as a reference while making this one. If you need more similar projects, see [this](https://github.com/topics/software-rendering) and [this](https://github.com/topics/software-rasterizer).

## References
- General Computer Graphics concepts:
    - https://www.scratchapixel.com/
    - https://learnopengl.com/
- Triangle rasteization:
    - https://www.youtube.com/watch?v=k5wtuKWmV48
    - https://dl.acm.org/doi/pdf/10.1145/54852.378457
    - https://acta.uni-obuda.hu/Mileff_Nehez_Dudra_63.pdf
    - https://www.montis.pmf.ac.me/allissues/47/Mathematica-Montisnigri-47-13.pdf
- Perspective-correct interpolation:
    - https://www.comp.nus.edu.sg/%7Elowkl/publications/lowk_persp_interp_techrep.pdf
    - https://www.youtube.com/watch?v=F5X6S35SW2s

And many, many more, all of which I will not find anymore...

## TODO
### Documentation/examples
- [x] Add comments to the examples
- [ ] Prettify the API (think about convenience macros/functions)
- [ ] Write the documentation about the main pipeline and individual functions

### Features
- [ ] Add interpolation with perspective correction
- [ ] Fix rasterization rules (see the gaps between triangles in `03_spinning_textured_cube` example)
- [ ] Implement other primitives (lines, points, lines/triangles strip/adjacency etc.)
- [ ] Implement interpolation for types other than `double` (shouldn't it just be `double` and `float`?)
- [ ] Add multisampling
- [ ] Advanced texture techniques:
    - [ ] (Bi)linear filtering
    - [ ] Mipmapping
    - [ ] Anisotropic filtering
    - [ ] Transparent textures?

### Optimization
- [ ] Are `vec` and `mat` functions inlined by the compiler? Should they be? Are `mat` or `vec` ever passed by-value?
- [ ] Use a profiler to find bottlenecks in frequently-used functions
- [ ] Draw primitives in parallel (OpenMP)

### Portability/API design
- [x] Give the user an opportunity to avoid including `vec.h` and `mat.h` (one more header guard?)
- [x] Use `const` in function declarations where needed
- [ ] Reorder arguments in drawcalls
- [ ] Hide the API structure definitions to avoid users modifying structure members themselves

## What I learned
This is a section specially for my portfolio, so feel free to skip it. Well, I learned:
- Advanced (I think so) C programming techniques:
    - Opaque types
    - Use of header guards to incapsulate internal functions & types
    - Many more...
- Importance of having a plan, an end goal:
    - This project started as something completely different from what it is now
- Basic Git workflow
- The importance of atomic (if it is possible) commits and meaningful commit names
- Premature optimization and optimization without measuring *really* is the root of all evil
- Importance of readable code and comments where they are needed

