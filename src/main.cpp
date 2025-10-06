
#include "display.h"
#include "vector.h"

#include <SDL.h>
#include <iostream>
#include <vector>

const int nPoints = 9 * 9 * 9;
std::vector<Vector3> cubePoints{ nPoints };
std::vector<Vector2> projectedCubePoints{ nPoints };

Display display;
bool isRunning = true;
Vector3 camPos = { 0,0,-5 };
Vector3 cubeRotation = { 0, 0, 0 };
int prevFrameTime = 0;
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
	display.colorBuffer = std::make_unique<uint32_t[]>(display.windowWidth * display.windowHeight);

	display.colorBufferTexture = SDL_CreateTexture(
		display.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, display.windowWidth, display.windowHeight);
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
	// Frame rate limiting
	int timeToWait = display.frameTargetTime - (SDL_GetTicks() - prevFrameTime);
	if (timeToWait > 0 && timeToWait <= display.frameTargetTime) {
		SDL_Delay(timeToWait);
	}
	prevFrameTime = SDL_GetTicks();

	cubeRotation.y += 0.01;
	for (int i = 0; i < nPoints; ++i) {
		Vector3 point = cubePoints[i];

		Vector3 transformedPoint = Vec3RotateY(point, cubeRotation.y);
		// move the point away from the camera
		transformedPoint.z -= camPos.z;

		Vector2 projectedPoint = Project(transformedPoint);
		projectedCubePoints[i] = projectedPoint;
	}
}

void Render() {
	display.ClearColorBuffer(0xFF00FF00);

	for (int i = 0; i < nPoints; ++i) {
		Vector2 projectePoint = projectedCubePoints[i];
		display.DrawRectangle(projectePoint.x + display.windowWidth / 2, projectePoint.y + display.windowHeight / 2, 4, 4, 0xFF0000FF);
	}

	display.RenderColorBuffer();

	SDL_RenderPresent(display.renderer);
}

int main(int argc, char* argv[]) {

	isRunning = display.InitWindow();

	Setup();

	Vector3 pos{ 1,1,1 };

	SDL_Event event;

	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}

	display.DestroyWindow();

	std::cout << "Application closed successfully!" << std::endl;
	return 0;
}
