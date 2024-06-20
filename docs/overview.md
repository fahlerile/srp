Overview {#mainpage}
========

SRP (stands for Software Rendering Pipeline) is an OpenGL-like CPU graphics library written in C with minimal dependencies. It was made as a hobby programming project, so it is probably not the best idea to use it in production code (see [PortableGL](https://github.com/rswinkle/PortableGL), for example). The source code (and more information) is available on [Github](https://www.github.com/fahlerile/srp).

A nice way to get familiar with API is to read <a href="examples.html">Examples</a> or <a href="group__API__reference.html">API reference</a>. If you are interested in implmentation details, see <a href="group__Implementation.html">Implementation</a>.

Here is a graph showing the pipeline (red edges - the main path from a draw call to a fragment on the screen, green nodes - objects created by user):

@dot
    graph {
        vertex_data [label="Vertex data"];
        index_data [label="Index data"];

        VertexBuffer [color=green];
        IndexBuffer [color=green];
        ShaderProgram [color=green];
        VertexShader [color=green];
        FragmentShader [color=green];
        Uniform [color=green];

        VertexBuffer -- vertex_data;
        IndexBuffer -- index_data;
        index_data -- vertex_data;
        vertex_data -- Drawcall;
        index_data -- Drawcall [style=dashed];
        ShaderProgram -- VertexShader,FragmentShader,Uniform;
        FragmentShader,VertexShader -- Uniform;

        edge [color=red];

        Drawcall -- VertexShader -- Rasterizer -- Interpolator -- FragmentShader;
    }
@enddot

