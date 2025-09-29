#pragma once

#include <SDL.h>

#include <memory>
#include <iostream>

// todo use some class for all that 
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern std::unique_ptr<uint32_t[]> colorBuffer;
extern SDL_Texture* colorBufferTexture;
extern int windowWidth;
extern int windowHeight;

bool InitWindow();
void DrawRectangle(int x, int y, int w, int h, uint32_t color);
void DrawGrid();
void RenderColorBuffer();
void ClearColorBuffer(uint32_t color);
void DestroyWindow();
void DrawPixel(int x, int y, uint32_t color);