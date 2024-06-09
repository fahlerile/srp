#include "Vector/Vector3.h"
#include "rasterizer.h"
#include "Matrix/Matrix.h"
#include "Window.h"
#include "timer.h"
#include "math_utils.h"
#include "log.h"

struct Vertex
{
	double position[3];
	double color[3];
};

struct Uniforms
{
	size_t frameCount;
	Matrix4 model;
	Matrix4 view;
	Matrix4 projection;
};

void vertexShader(VSInput* in, VSOutput* out);
void fragmentShader(FSInput* in, FSOutput* out);

int main()
{
	Framebuffer* fb = newFramebuffer(512, 512);

	Vertex data[8] = {
		{.position = {-1., -1., -1.}, .color = {1., 0., 0.}},
		{.position = {-1., -1.,  1.}, .color = {0., 1., 0.}},
		{.position = { 1., -1.,  1.}, .color = {0., 0., 1.}},
		{.position = { 1., -1., -1.}, .color = {1., 1., 0.}},
		{.position = {-1.,  1., -1.}, .color = {0., 1., 1.}},
		{.position = {-1.,  1.,  1.}, .color = {1., 0., 1.}},
		{.position = { 1.,  1.,  1.}, .color = {1., 1., 1.}},
		{.position = { 1.,  1., -1.}, .color = {0.8, 0.2, 0.5}},
	};
	uint8_t indices[36] = {
		3, 6, 7,  3, 2, 6,  // +x
		1, 4, 5,  1, 0, 4,  // -x
		5, 7, 6,  5, 4, 7,  // +y
		0, 2, 3,  0, 1, 2,  // -y
		2, 5, 6,  2, 1, 5,  // +z
		0, 7, 4,  0, 3, 7   // -z
	};

	VertexVariable VSOutputVariables[1] = {
		{
			.nItems = 3,
			.type = TYPE_DOUBLE,
			.offsetBytes = 0
		}
	};

	VertexBuffer* vb = newVertexBuffer(sizeof(Vertex), sizeof(data), data);
	IndexBuffer* ib = newIndexBuffer(TYPE_UINT8, sizeof(indices), indices);

	Uniforms uniforms = {
		.model = Matrix4ConstructIdentity(),
		.view = Matrix4ConstructView(
			(Vector3d) {0, 0, -3}, (Vector3d) {0},
			(Vector3d) {1, 1, 1}
		),
		.projection = Matrix4ConstructPerspectiveProjection(-1, 1, -1, 1, 1, 50),
		.frameCount = 0
	};
	ShaderProgram shaderProgram = {
		.uniforms = &uniforms,
		.vs = {
			.shader = vertexShader,
			.nBytesPerOutputVariables = sizeof(Vertex),
			.nOutputVariables = 1,
			.outputVariables = VSOutputVariables,
		},
		.fs = {
			.shader = fragmentShader
		}
	};

	Window* window = newWindow(512, 512, "Rasterizer", false);

	while (window->running)
	{
		TIMER_START(frametime);

		uniforms.model = Matrix4ConstructRotate((Vector3d) {
			uniforms.frameCount / 100.,
			uniforms.frameCount / 200.,
			uniforms.frameCount / 500.,
		});
		framebufferClear(fb);
		drawIndexBuffer(fb, ib, vb, PRIMITIVE_TRIANGLES, 0, 36, &shaderProgram);

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


void vertexShader(VSInput* in, VSOutput* out)
{
	double* pos = in->pVertex->position;
	out->position = (Vector4d) {
		pos[0], pos[1], pos[2], 1.0
	};
	out->position = Matrix4MultiplyVector4d(&in->uniforms->model, out->position);
	out->position = Matrix4MultiplyVector4d(&in->uniforms->view, out->position);
	out->position = Matrix4MultiplyVector4d(&in->uniforms->projection, out->position);

	double* colorOut = (double*) out->pOutputVariables;
	colorOut[0] = in->pVertex->color[0];
	colorOut[1] = in->pVertex->color[1];
	colorOut[2] = in->pVertex->color[2];
}

void fragmentShader(FSInput* in, FSOutput* out)
{
	double* colorIn = (double*) in->interpolated;
	memcpy(&out->color, colorIn, 3 * sizeof(double));
	out->color.w = 1.;
}

