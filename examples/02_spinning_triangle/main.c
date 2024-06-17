#define SRP_INCLUDE_VEC
#define SRP_INCLUDE_MAT

#include <stdio.h>
#include "srp.h"
#include "window.h"
#include "timer.h"
#include "rad.h"

typedef struct Vertex
{
	double position[3];
	double color[3];
} Vertex;

typedef struct Uniform
{
	size_t frameCount;
	mat4d rotation;
} Uniform;

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
	srpContextSetP(CTX_PARAM_MESSAGE_CALLBACK, (void*) messageCallback);

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

	SRPVertexVariable VSOutputVariables[1] = {
		{
			.nItems = 3,
			.type = TYPE_DOUBLE,
			.offsetBytes = 0
		}
	};

	SRPVertexBuffer* vb = srpNewVertexBuffer(sizeof(Vertex), sizeof(data), data);
	SRPIndexBuffer* ib = srpNewIndexBuffer(TYPE_UINT8, sizeof(indices), indices);

	Uniform uniform = {0};
	SRPShaderProgram shaderProgram = {
		.uniform = (SRPUniform*) &uniform,
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

	while (window->running)
	{
		TIMER_START(frametime);

		uniform.rotation = mat4dConstructRotate(0, 0, uniform.frameCount / 1000.);
		framebufferClear(fb);
		srpDrawIndexBuffer(fb, ib, vb, PRIMITIVE_TRIANGLES, 0, 3, &shaderProgram);

		windowPollEvents(window);
		windowPresent(window, fb);

		uniform.frameCount++;
		TIMER_STOP(frametime);
		printf(
			"Frametime: %li us; FPS: %lf; Framecount: %zu\n",
			TIMER_REPORT_US(frametime, long),
			1. / TIMER_REPORT_S(frametime, double),
			uniform.frameCount
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
	Uniform* pUniform = (Uniform*) in->uniform;

	vec3d* inPosition = (vec3d*) pVertex->position;
	vec4d* outPosition = (vec4d*) out->position;
	*outPosition = (vec4d) {
		inPosition->x, inPosition->y, inPosition->z, 1.0
	};
	*outPosition = mat4dMultiplyVec4d(&pUniform->rotation, *outPosition);

	double* colorOut = (double*) out->pOutputVariables;
	colorOut[0] = pVertex->color[0] + sin(pUniform->frameCount * 2.5e-3) * 0.3;
	colorOut[1] = pVertex->color[1] + sin(pUniform->frameCount * 0.5e-3) * 0.1;
	colorOut[2] = pVertex->color[2] + sin(pUniform->frameCount * 5e-3) * 0.5;
}

void fragmentShader(SRPfsInput* in, SRPfsOutput* out)
{
	double* colorIn = (double*) in->interpolated;
	memcpy(&out->color, colorIn, 3 * sizeof(double));
	out->color[3] = 1.;
}

