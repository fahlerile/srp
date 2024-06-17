#include <stdio.h>
#include "srp.h"
#include "window.h"
#include "timer.h"
#include "mat.h"

typedef struct Vertex
{
	double position[3];
	double uv[2];
} Vertex;

typedef struct Uniform
{
	size_t frameCount;
	mat4d model;
	mat4d view;
	mat4d projection;
	SRPTexture* texture;
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
	srpContextSetP(CTX_PARAM_MESSAGE_CALLBACK, (void*) &messageCallback);

	SRPFramebuffer* fb = srpNewFramebuffer(512, 512);

	Vertex data[] = {
		{.position = {-1, -1, -1}, .uv = {0, 0}},
		{.position = { 1, -1, -1}, .uv = {1, 0}},
		{.position = { 1,  1, -1}, .uv = {1, 1}},
		{.position = {-1,  1, -1}, .uv = {0, 1}},

		{.position = {-1,  1, -1}, .uv = {0, 0}},
		{.position = { 1,  1, -1}, .uv = {1, 0}},
		{.position = { 1,  1,  1}, .uv = {1, 1}},
		{.position = {-1,  1,  1}, .uv = {0, 1}},

		{.position = { 1, -1,  1}, .uv = {0, 0}},
		{.position = {-1, -1,  1}, .uv = {1, 0}},
		{.position = {-1,  1,  1}, .uv = {1, 1}},
		{.position = { 1,  1,  1}, .uv = {0, 1}},

		{.position = { 1, -1,  1}, .uv = {0, 0}},
		{.position = { 1, -1, -1}, .uv = {1, 0}},
		{.position = { 1,  1, -1}, .uv = {1, 1}},
		{.position = { 1,  1,  1}, .uv = {0, 1}},

		{.position = {-1, -1, -1}, .uv = {0, 0}},
		{.position = {-1, -1,  1}, .uv = {1, 0}},
		{.position = {-1,  1,  1}, .uv = {1, 1}},
		{.position = {-1,  1, -1}, .uv = {0, 1}},
		
		{.position = {-1, -1, -1}, .uv = {0, 0}},
		{.position = { 1, -1, -1}, .uv = {1, 0}},
		{.position = { 1, -1,  1}, .uv = {1, 1}},
		{.position = {-1, -1,  1}, .uv = {0, 1}}
	};
	uint8_t indices[] = {
		0, 1, 2,  0, 2, 3,
		4, 5, 6,  4, 6, 7,
		8, 9, 10,  8, 10, 11,
		12, 15, 14,  12, 14, 13,
		16, 18, 17,  16, 19, 18,
		20, 23, 22,  20, 22, 21
	};

	SRPVertexBuffer* vb = srpNewVertexBuffer(sizeof(Vertex), sizeof(data), data);
	SRPIndexBuffer* ib = srpNewIndexBuffer(TYPE_UINT8, sizeof(indices), indices);

	SRPVertexVariable VSOutputVariables[1] = {
		{
			.nItems = 2,
			.type = TYPE_DOUBLE,
			.offsetBytes = 0
		}
	};

	Uniform uniform = {
		.model = mat4dConstructIdentity(),
		.view = mat4dConstructView(
			0, 0, -3,
			0, 0, 0,
			1, 1, 1
		),
		.projection = mat4dConstructPerspectiveProjection(-1, 1, -1, 1, 1, 50),
		.texture = srpNewTexture(
			"./res/textures/stoneWall.png", TW_REPEAT, TW_REPEAT,
			TF_NEAREST, TF_NEAREST
		),
		.frameCount = 0
	};

	SRPShaderProgram shaderProgram = {
		.uniform = (SRPUniform*) &uniform,
		.vs = {
			.shader = vertexShader,
			.nBytesPerOutputVariables = sizeof(double) * 2,
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

		uniform.model = mat4dConstructRotate(
			uniform.frameCount / 100.,
			uniform.frameCount / 200.,
			uniform.frameCount / 500.
		);
		framebufferClear(fb);
		srpDrawIndexBuffer(fb, ib, vb, PRIMITIVE_TRIANGLES, 0, 36, &shaderProgram);

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

	srpFreeTexture(uniform.texture);
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

	double* pos = pVertex->position;
	out->position = (vec4d) {
		pos[0], pos[1], pos[2], 1.0
	};
	out->position = mat4dMultiplyVec4d(&pUniform->model, out->position);
	out->position = mat4dMultiplyVec4d(&pUniform->view, out->position);
	out->position = mat4dMultiplyVec4d(&pUniform->projection, out->position);

	double* uvOut = (double*) out->pOutputVariables;
	uvOut[0] = pVertex->uv[0];
	uvOut[1] = pVertex->uv[1];
}

void fragmentShader(SRPfsInput* in, SRPfsOutput* out)
{
	Uniform* pUniform = (Uniform*) in->uniform;

	double* uv = (double*) in->interpolated;
	SRPColor color = srpTextureGetFilteredColor(pUniform->texture, uv[0], uv[1]);
	out->color.x = color.r / 255.;
	out->color.y = color.g / 255.;
	out->color.z = color.b / 255.;
	out->color.w = color.a / 255.;
}

