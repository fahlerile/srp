#include <stdio.h>
#include "rasterizer.h"
#include "Window.h"
#include "timer.h"
#include "rad.h"

typedef struct Vertex
{
	double position[3];
	double color[3];
} Vertex;

SRPContext srpContext;

void messageCallback(
	SRPMessageType type, SRPMessageSeverity severity, const char* sourceFunction,
	const char* message, void* userParameter
);
void vertexShader(SRPvsInput* in, SRPvsOutput* out);
void fragmentShader(SRPfsInput* in, SRPfsOutput* out);

int main()
{
	srpNewContext(&srpContext);
	srpContextSetP(CTX_PARAM_MESSAGE_CALLBACK, (void**) &messageCallback);

	SRPFramebuffer* fb = srpNewFramebuffer(512, 512);

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

	SRPVertexBuffer* vb = srpNewVertexBuffer(sizeof(Vertex), sizeof(data), data);
	SRPIndexBuffer* ib = srpNewIndexBuffer(TYPE_UINT8, sizeof(indices), indices);

	SRPVertexVariable VSOutputVariables[1] = {
		{
			.nItems = 3,
			.type = TYPE_DOUBLE,
			.offsetBytes = 0
		}
	};

	SRPShaderProgram shaderProgram = {
		.uniform = NULL,
		.vs = {
			.shader = vertexShader,
			.nBytesPerOutputVariables = sizeof(double) * 3,
			.nOutputVariables = 1,
			.outputVariables = VSOutputVariables,
		},
		.fs = {
			.shader = fragmentShader
		}
	};

	Window* window = newWindow(512, 512, "Rasterizer", false);

	size_t frameCount = 0;
	while (window->running)
	{
		TIMER_START(frametime);

		framebufferClear(fb);
		srpDrawIndexBuffer(fb, ib, vb, PRIMITIVE_TRIANGLES, 0, 3, &shaderProgram);

		windowPollEvents(window);
		windowPresent(window, fb);

		frameCount++;
		TIMER_STOP(frametime);
		printf(
			"Frametime: %li us; FPS: %lf; Framecount: %zu\n",
			TIMER_REPORT_US(frametime, long),
			1. / TIMER_REPORT_S(frametime, double),
			frameCount
		);
	}

	srpFreeVertexBuffer(vb);
	srpFreeIndexBuffer(ib);
	srpFreeFramebuffer(fb);
	freeWindow(window);

	return 0;
}


void messageCallback(
	SRPMessageType type, SRPMessageSeverity severity, const char* sourceFunction,
	const char* message, void* userParameter
)
{
	fprintf(stderr, "%s: %s", sourceFunction, message);
}


void vertexShader(SRPvsInput* in, SRPvsOutput* out)
{
	Vertex* pVertex = (Vertex*) in->pVertex;

	double* pos = pVertex->position;
	out->position = (vec4d) {
		pos[0], pos[1], pos[2], 1.0
	};

	double* colorOut = (double*) out->pOutputVariables;
	colorOut[0] = pVertex->color[0];
	colorOut[1] = pVertex->color[1];
	colorOut[2] = pVertex->color[2];
}

void fragmentShader(SRPfsInput* in, SRPfsOutput* out)
{
	double* colorIn = (double*) in->interpolated;
	memcpy(&out->color, colorIn, 3 * sizeof(double));
	out->color.w = 1.;
}

