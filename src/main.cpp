
#include "display.h"
#include "vector.h"

#include <SDL.h>
#include <iostream>
#include <vector>

const int nPoints = 9 * 9 * 9;
std::vector<Vector3> cubePoints{ nPoints };

bool isRunning = true;
SDL_Window* window;
SDL_Renderer* renderer;
std::unique_ptr<uint32_t[]> colorBuffer = nullptr;
SDL_Texture* colorBufferTexture;
int windowWidth = 800;
int windowHeight = 600;

void ProcessInput() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			isRunning = false;
		break;
	}
}

void Setup() {
	colorBuffer = std::make_unique<uint32_t[]>(windowWidth * windowHeight);

	colorBufferTexture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);

	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				Vector3 newPoint = { x,y,z };
				cubePoints.push_back(newPoint);
			}
		}
	}
}


void Update() {

}


void Render() {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	ClearColorBuffer(0xFF00FF00);
	DrawGrid();
	DrawRectangle(100, 50, 100, 100, 0xFFFF00FF);
	DrawPixel(20, 20, 0xFFFF00FF);
	RenderColorBuffer();

	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {

	isRunning = InitWindow();

	Setup();

	Vector3 pos{ 1,1,1 };

	SDL_Event event;

	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}

	DestroyWindow();

	std::cout << "Application closed successfully!" << std::endl;
	return 0;
}