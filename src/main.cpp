
#include "display.h"
#include "vector.h"
#include "mesh.h"

#include <SDL.h>
#include <iostream>
#include <vector>

Display display;
bool isRunning = true;
Vector3 camPos = { 0,0,-5 };
Vector3 cubeRotation = { 0, 0, 0 };
int prevFrameTime = 0;
int fovFactor = 640;

Triangle trianglesToRender[N_MESH_FACES];

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
}

void Update() {
	// Frame rate limiting
	int timeToWait = display.frameTargetTime - (SDL_GetTicks() - prevFrameTime);
	if (timeToWait > 0 && timeToWait <= display.frameTargetTime) {
		SDL_Delay(timeToWait);
	}
	prevFrameTime = SDL_GetTicks();

	cubeRotation.y += 0.01;

	for (int i = 0; i < N_MESH_FACES; ++i) {
		Face meshFace = meshFaces[i]; // indices of our vertices 
		Vector3 faceVertices[3];	  // Set of vertices
		faceVertices[0] = meshVertices[meshFace.a - 1];
		faceVertices[1] = meshVertices[meshFace.b - 1];
		faceVertices[2] = meshVertices[meshFace.c - 1];
		Triangle projectedTriangle; // 3 Vertices with 2D coordinates
		for (int j = 0; j < 3; ++j) {
			// Apply rotation for fun
			Vector3 transformedPoint = Vec3RotateY(faceVertices[j], cubeRotation.y);
			// Translate the vertex away from the camera in z
			transformedPoint.z -= camPos.z;
			// Project using perspective projection
			Vector2 projectedPoint = Project(transformedPoint);
			// Scale and translate the projected points to the middle of the screen
			projectedPoint.x += display.windowWidth / 2;
			projectedPoint.y += display.windowHeight / 2;
			projectedTriangle.points[j] = projectedPoint;
		}
		trianglesToRender[i] = projectedTriangle;
	}
}

void Render() {
	display.ClearColorBuffer(0xFF00FF00);

	for (int i = 0; i < N_MESH_FACES; ++i) {
		Triangle triangle = trianglesToRender[i];
		display.DrawRectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, 0XFFFFFF00);
		display.DrawRectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, 0XFFFFFF00);
		display.DrawRectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, 0XFFFFFF00);
	}

	display.RenderColorBuffer();

	SDL_RenderPresent(display.renderer);
}

int main(int argc, char* argv[]) {

	isRunning = display.InitWindow();

	Setup();

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
