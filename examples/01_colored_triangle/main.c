#include "rasterizer.h"
#include "Window.h"
#include "timer.h"

struct Vertex
{
	double position[3];
	double color[3];
};

void vertexShader(
	const ShaderProgram* sp, const Vertex* pVertex, VSOutput* pOutput,
	size_t vertexIndex
);
void fragmentShader
	(const ShaderProgram* sp, const Interpolated* pFragment, Color* pColor);

int main()
{
	Framebuffer* fb = newFramebuffer(512, 512);

	Vertex data[3] = {
		{.position = {-0.75, -0.75, 0.}, .color = {1., 0., 0.}},
		{.position = { 0.75, -0.75, 0.}, .color = {0., 1., 0.}},
		{.position = { 0.  ,  0.75, 0.}, .color = {0., 0., 1.}}
	};
	uint8_t indices[3] = {
		0, 1, 2
	};

	VertexAttribute attributes[2] = {
		{
			.nItems = 3,
			.type = TYPE_DOUBLE,
			.offsetBytes = offsetof(Vertex, position)
		},
		{
			.nItems = 3,
			.type = TYPE_DOUBLE,
			.offsetBytes = offsetof(Vertex, color)
		}
	};

	VertexBuffer* vb = \
		newVertexBuffer(sizeof(Vertex), sizeof(data), data, 2, attributes);
	IndexBuffer* ib = newIndexBuffer(TYPE_UINT8, sizeof(indices), indices);

	ShaderProgram shaderProgram = {
		.uniforms = NULL,
		.vertexShader = {
			.shader = vertexShader,
			.nBytesPerOutputVertex = sizeof(Vertex),
			.nOutputAttributes = 2,
			.outputAttributes = attributes,
			.indexOfOutputPositionAttribute = 0
		},
		.geometryShader = {0},
		.fragmentShader = {
			.shader = fragmentShader
		}
	};
	shaderProgramSetDefaultGeometryShader(&shaderProgram);

	Window* window = newWindow(512, 512, "Rasterizer", false);

	size_t frameCount = 0;
	while (window->running)
	{
		TIMER_START(frametime);

		framebufferClear(fb);
		drawIndexBuffer(fb, ib, vb, PRIMITIVE_TRIANGLES, 0, 3, &shaderProgram);

		windowPollEvents(window);
		windowPresent(window, fb);

		frameCount++;
		TIMER_STOP(frametime);
		LOGI(
			"Frametime: %li us; FPS: %lf; Framecount: %zu\n",
			TIMER_REPORT_US(frametime, long),
			1. / TIMER_REPORT_S(frametime, double),
			frameCount
		);
	}

	freeVertexBuffer(vb);
	freeIndexBuffer(ib);
	freeFramebuffer(fb);
	freeWindow(window);

	return 0;
}


void vertexShader(
	const ShaderProgram* sp, const Vertex* pVertex, VSOutput* pOutput,
	size_t vertexIndex
)
{
	*(Vertex*) pOutput = *pVertex;
}

void fragmentShader
	(const ShaderProgram* sp, const Interpolated* pFragment, Color* pColor)
{
	double* colorVec = ((Vertex*) pFragment)->color;
	*pColor = (Color) {
		colorVec[0] * 255,
		colorVec[1] * 255,
		colorVec[2] * 255,
		255
	};
}

