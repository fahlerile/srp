#include <stdio.h>
#include "rasterizer.h"
#include "Window.h"
#include "timer.h"
#include "mat.h"

typedef struct Vertex
{
	double position[3];
	double uv[2];
} Vertex;

typedef struct Uniforms
{
	size_t frameCount;
	mat4d model;
	mat4d view;
	mat4d projection;
	Texture* texture;
} Uniforms;

void vertexShader(VSInput* in, VSOutput* out);
void fragmentShader(FSInput* in, FSOutput* out);

int main()
{
	Framebuffer* fb = newFramebuffer(512, 512);

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

	VertexBuffer* vb = newVertexBuffer(sizeof(Vertex), sizeof(data), data);
	IndexBuffer* ib = newIndexBuffer(TYPE_UINT8, sizeof(indices), indices);

	VertexVariable VSOutputVariables[1] = {
		{
			.nItems = 2,
			.type = TYPE_DOUBLE,
			.offsetBytes = 0
		}
	};

	Uniforms uniforms = {
		.model = mat4dConstructIdentity(),
		.view = mat4dConstructView(
			0, 0, -3,
			0, 0, 0,
			1, 1, 1
		),
		.projection = mat4dConstructPerspectiveProjection(-1, 1, -1, 1, 1, 50),
		.texture = newTexture(
			"./res/textures/stoneWall.png", TW_REPEAT, TW_REPEAT,
			TF_NEAREST, TF_NEAREST
		),
		.frameCount = 0
	};

	ShaderProgram shaderProgram = {
		.uniforms = &uniforms,
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

		uniforms.model = mat4dConstructRotate(
			uniforms.frameCount / 100.,
			uniforms.frameCount / 200.,
			uniforms.frameCount / 500.
		);
		framebufferClear(fb);
		drawIndexBuffer(fb, ib, vb, PRIMITIVE_TRIANGLES, 0, 36, &shaderProgram);

		windowPollEvents(window);
		windowPresent(window, fb);

		uniforms.frameCount++;
		TIMER_STOP(frametime);
		printf(
			"Frametime: %li us; FPS: %lf; Framecount: %zu\n",
			TIMER_REPORT_US(frametime, long),
			1. / TIMER_REPORT_S(frametime, double),
			uniforms.frameCount
		);
	}

	freeTexture(uniforms.texture);
	freeVertexBuffer(vb);
	freeIndexBuffer(ib);
	freeFramebuffer(fb);
	freeWindow(window);

	return 0;
}


void vertexShader(VSInput* in, VSOutput* out)
{
	double* pos = in->pVertex->position;
	out->position = (vec4d) {
		pos[0], pos[1], pos[2], 1.0
	};
	out->position = mat4dMultiplyVec4d(&in->uniforms->model, out->position);
	out->position = mat4dMultiplyVec4d(&in->uniforms->view, out->position);
	out->position = mat4dMultiplyVec4d(&in->uniforms->projection, out->position);

	double* uvOut = (double*) out->pOutputVariables;
	uvOut[0] = in->pVertex->uv[0];
	uvOut[1] = in->pVertex->uv[1];
}

void fragmentShader(FSInput* in, FSOutput* out)
{
	double* uv = (double*) in->interpolated;
	Color color = textureGetFilteredColor(in->uniforms->texture, uv[0], uv[1]);
	out->color = (vec4d) {
		(double) color.r / 255,
		(double) color.g / 255,
		(double) color.b / 255,
		(double) color.a / 255
	};
}

