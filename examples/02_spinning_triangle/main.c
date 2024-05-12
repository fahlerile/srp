#include "rasterizer.h"
#include "Matrix/Matrix.h"
#include "Window.h"
#include "timer.h"
#include "utils.h"

struct Vertex
{
	double position[3];
	double color[3];
};

struct Uniforms
{
	size_t frameCount;
	Matrix4 rotation;
};

void vertexShader(
	const ShaderProgram* sp, const Vertex* pVertex, VSOutput* pOutput,
	size_t vertexIndex
);
void fragmentShader
	(const ShaderProgram* sp, const Interpolated* pFragment, double* pColor);

int main()
{
	Framebuffer* fb = newFramebuffer(512, 512);

	const double R = 0.8;
	Vertex data[3] = {
		{.position = {0., R, 0.}, .color = {1., 0., 0.}},
		{
			.position = {-cos(RADIANS(30)) * R, -sin(RADIANS(30)) * R, 0.},
			.color = {0., 0., 1.}
		},
		{
			.position = { cos(RADIANS(30)) * R, -sin(RADIANS(30)) * R, 0.},
			.color = {0., 1., 0.}
		}
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

	Uniforms uniforms = {0};
	ShaderProgram shaderProgram = {
		.uniforms = &uniforms,
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

	while (window->running)
	{
		TIMER_START(frametime);

		uniforms.rotation = Matrix4ConstructRotate((Vector3d) {
			0, 0, uniforms.frameCount / 1000.
		});
		framebufferClear(fb);
		drawIndexBuffer(fb, ib, vb, PRIMITIVE_TRIANGLES, 0, 3, &shaderProgram);

		windowPollEvents(window);
		windowPresent(window, fb);

		uniforms.frameCount++;
		TIMER_STOP(frametime);
		LOGI(
			"Frametime: %li us; FPS: %lf; Framecount: %zu\n",
			TIMER_REPORT_US(frametime, long),
			1. / TIMER_REPORT_S(frametime, double),
			uniforms.frameCount
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
	Vector4d position = Matrix4MultiplyVector4d(
		&sp->uniforms->rotation,
		*(Vector4d*) pVertex->position
	);
	memcpy(
		((Vertex*) pOutput)->position,
		&position,
		sizeof(pVertex->position)
	);

	((Vertex*) pOutput)->color[0] = \
		pVertex->color[0] + sin(sp->uniforms->frameCount * 2.5e-3) * 0.3;
	((Vertex*) pOutput)->color[1] = \
		pVertex->color[1] + sin(sp->uniforms->frameCount * 0.5e-3) * 0.1;
	((Vertex*) pOutput)->color[2] = \
		pVertex->color[2] + sin(sp->uniforms->frameCount * 5e-3) * 0.5;
}

void fragmentShader
	(const ShaderProgram* sp, const Interpolated* pFragment, double* pColor)
{
	memcpy(
		pColor,
		((Vertex*) pFragment)->color,
		3 * sizeof(double)
	);
	pColor[3] = 1.;
}

