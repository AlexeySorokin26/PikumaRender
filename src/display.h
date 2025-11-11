#pragma once

#include <SDL.h>

#include "vector.h"

#include <memory>
#include <iostream>

enum class CullMethod {
	CULL_NONE,
	CULL_BACKFACE
};

enum class RenderMethod {
	RENDER_VERTEX,
	RENDER_WIRE,
	RENDER_WIRE_VERTEX,
	RENDER_FILL_TRIANGLE,
	RENDER_FILL_TRIANGLE_VERTEX,
	RENDER_FILL_TRIANGLE_WIRE,
	RENDER_FILL_TRIANGLE_WIRE_VERTEX,
	RENDER_TEXTURED,
	RENDER_TEXTURED_WIRED,
};

class Display {
public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unique_ptr<uint32_t[]> colorBuffer;
	SDL_Texture* colorBufferTexture;
	int windowWidth = 800;
	int windowHeight = 600;
	const int FPS = 30; // we want to render 30 frames in 1 second (it's just a defintion)
	const float frameTargetTime = (1000.0 / FPS); // how many time each frame should take if we want to render all in 1s
	CullMethod cullMethod = CullMethod::CULL_NONE;
	RenderMethod renderMethod = RenderMethod::RENDER_WIRE;

public:
	bool InitWindow();
	void DrawRectangle(int x, int y, int w, int h, uint32_t color);
	void DrawGrid();
	void RenderColorBuffer();
	void ClearColorBuffer(uint32_t color);
	void DestroyWindow();
	void DrawPixel(int x, int y, uint32_t color);
	void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);
	void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
	void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
	void DrawTexturedTriangle(
		int x0, int y0, float u0, float v0,
		int x1, int y1, float u1, float v1,
		int x2, int y2, float u2, float v2,
		uint32_t* texture
	);
	void FillFlatTopTriangle(int x0, int y0, int Mx, int My, int x2, int y2, uint32_t color);
	void FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int Mx, int My, uint32_t color);
	Vector3 BarycentricWeights(Vector2 a, Vector2 b, Vector2 c, Vector2 p);
	void DrawTexel(int x, int y, uint32_t* texture, Vector2 pointA, Vector2 pointB, Vector2 pointC,
		float u0, float vo, float u1, float v1, float u2, float v2);
};



