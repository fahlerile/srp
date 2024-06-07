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
	Matrix4 rotation;
};

void vertexShader(VSInput* in, VSOutput* out);
void fragmentShader(FSInput* in, FSOutput* out);

int main()
{
	Framebuffer* fb = newFramebuffer(512, 512);

	const double R = 0.8;
	Vertex data[3] = {
		{.position = {0., R, 0.}, .color = {1., 0., 0.}},
		{
			.position = {-cos(RAD(30)) * R, -sin(RAD(30)) * R, 0.},
			.color = {0., 0., 1.}
		},
		{
			.position = { cos(RAD(30)) * R, -sin(RAD(30)) * R, 0.},
			.color = {0., 1., 0.}
		}
	};
	uint8_t indices[3] = {
		0, 1, 2
	};

	VertexVariable VSOutputVariables[1] = {
		{
			.nItems = 3,
			.type = TYPE_DOUBLE,
			.offsetBytes = 0
		}
	};

	VertexBuffer* vb = \
		newVertexBuffer(sizeof(Vertex), sizeof(data), data);
	IndexBuffer* ib = newIndexBuffer(TYPE_UINT8, sizeof(indices), indices);

	Uniforms uniforms = {0};
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


void vertexShader(VSInput* in, VSOutput* out)
{
	double* pos = in->pVertex->position;
	out->position = (Vector4d) {
		pos[0], pos[1], pos[2], 1.0
	};
	out->position = Matrix4MultiplyVector4d(&in->uniforms->rotation, out->position);

	double* colorOut = (double*) out->pOutputVariables;
	colorOut[0] = in->pVertex->color[0] + sin(in->uniforms->frameCount * 2.5e-3) * 0.3;
	colorOut[1] = in->pVertex->color[1] + sin(in->uniforms->frameCount * 0.5e-3) * 0.1;
	colorOut[2] = in->pVertex->color[2] + sin(in->uniforms->frameCount * 5e-3) * 0.5;
}

void fragmentShader(FSInput* in, FSOutput* out)
{
	double* colorIn = (double*) in->interpolated;
	memcpy(&out->color, colorIn, 3 * sizeof(double));
	out->color.w = 1.;
}

