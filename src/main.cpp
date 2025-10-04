
#include "display.h"
#include "vector.h"

#include <SDL.h>
#include <iostream>
#include <vector>

const int nPoints = 9 * 9 * 9;
std::vector<Vector3> cubePoints{ nPoints };
std::vector<Vector2> projectedCubePoints{ nPoints };

bool isRunning = true;
SDL_Window* window;
SDL_Renderer* renderer;
std::unique_ptr<uint32_t[]> colorBuffer = nullptr;
SDL_Texture* colorBufferTexture;
Vector3 camPos = { 0,0,-5 };
int windowWidth = 800;
int windowHeight = 600;
int fovFactor = 640;

Vector2 Project(Vector3 point) {
	Vector2 projectedPoint{ fovFactor * point.x / point.z, fovFactor * point.y / point.z };
	return projectedPoint;
}

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
	int pointCounter = 0;
	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				Vector3 newPoint = { x,y,z };
				cubePoints[pointCounter++] = newPoint;
			}
		}
	}
}

void Update() {
	for (int i = 0; i < nPoints; ++i) {
		Vector3 point = cubePoints[i];

		// move the point away from the camera
		point.z -= camPos.z;

		Vector2 projectedPoint = Project(point);
		projectedCubePoints[i] = projectedPoint;
	}
}

void Render() {
	ClearColorBuffer(0xFF00FF00);

	for (int i = 0; i < nPoints; ++i) {
		Vector2 projectePoint = projectedCubePoints[i];
		DrawRectangle(projectePoint.x + windowWidth / 2, projectePoint.y + windowHeight / 2, 4, 4, 0xFF0000FF);
	}

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