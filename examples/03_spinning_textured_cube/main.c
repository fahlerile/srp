#define SRP_INCLUDE_VEC
#define SRP_INCLUDE_MAT

#include <stdio.h>
#include "srp.h"
#include "window.h"
#include "timer.h"

typedef struct Vertex
{
	vec3d position;
	vec2d uv;
} Vertex;

typedef struct VSOutput
{
	vec2d uv;
} VSOutput;

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
		 0,  1,  2,   0,  2,  3,
		 4,  5,  6,   4,  6,  7,
		 8,  9, 10,   8, 10, 11,
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
			"./res/textures/stoneWall.png",
			TW_REPEAT, TW_REPEAT,
			TF_NEAREST, TF_NEAREST
		),
		.frameCount = 0
	};

	SRPShaderProgram shaderProgram = {
		.uniform = (SRPUniform*) &uniform,
		.vs = {
			.shader = vertexShader,
			.nBytesPerOutputVariables = sizeof(VSOutput),
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
	VSOutput* pOutVars = (VSOutput*) out->pOutputVariables;

	vec3d* inPosition = &pVertex->position;
	vec4d* outPosition = (vec4d*) out->position;
	*outPosition = (vec4d) {
		inPosition->x, inPosition->y, inPosition->z, 1.0
	};
	*outPosition = mat4dMultiplyVec4d(&pUniform->model, *outPosition);
	*outPosition = mat4dMultiplyVec4d(&pUniform->view, *outPosition);
	*outPosition = mat4dMultiplyVec4d(&pUniform->projection, *outPosition);

	pOutVars->uv.x = pVertex->uv.x;
	pOutVars->uv.y = pVertex->uv.y;
}

void fragmentShader(SRPfsInput* in, SRPfsOutput* out)
{
	VSOutput* interpolated = (VSOutput*) in->interpolated;
	Uniform* pUniform = (Uniform*) in->uniform;
	vec4d* outColor = (vec4d*) out->color;

	vec2d uv = interpolated->uv;
	SRPColor color = srpTextureGetFilteredColor(pUniform->texture, uv.x, uv.y);
	outColor->x = color.r / 255.;
	outColor->y = color.g / 255.;
	outColor->z = color.b / 255.;
	outColor->w = color.a / 255.;
}

