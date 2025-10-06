#pragma once

#include <SDL.h>

#include <memory>
#include <iostream>

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

public:
	bool InitWindow();
	void DrawRectangle(int x, int y, int w, int h, uint32_t color);
	void DrawGrid();
	void RenderColorBuffer();
	void ClearColorBuffer(uint32_t color);
	void DestroyWindow();
	void DrawPixel(int x, int y, uint32_t color);
};



