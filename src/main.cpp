
#include "display.h"
#include "vector.h"
#include "mesh.h"

#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>

Display display;
bool isRunning = true;
Vector3 camPos = { 0,0,0 };
int prevFrameTime = 0;
int fovFactor = 640;
Mesh mesh;
std::string objPath;

std::vector<Triangle> trianglesToRender;

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
        if (event.key.keysym.sym == SDLK_1)
            display.renderMethod = RenderMethod::RENDER_WIRE_VERTEX;
        if (event.key.keysym.sym == SDLK_2)
            display.renderMethod = RenderMethod::RENDER_WIRE;
        if (event.key.keysym.sym == SDLK_3)
            display.renderMethod = RenderMethod::RENDER_FILL_TRIANGLE;
        if (event.key.keysym.sym == SDLK_4)
            display.renderMethod = RenderMethod::RENDER_FILL_TRIANGLE_WIRE;
        if (event.key.keysym.sym == SDLK_c)
            display.cullMethod = CullMethod::CULL_BACKFACE;
        if (event.key.keysym.sym == SDLK_d)
            display.cullMethod = CullMethod::CULL_NONE;
        break;
	}
}

void Setup() {
	//mesh.LoadCube();
    // objPath = "C:\\Users\\PC\\Desktop\\Pikuma\\assets\\cube.obj"; // todo cool to have some resources path
    objPath = "/Users/maestro/Desktop/PikumaRender/assets/cube.obj"; // todo cool to have some resources path
	mesh.LoadObjFile(objPath);
	trianglesToRender.resize(mesh.faces.size()); // Allocate and call constructor

	display.colorBuffer = std::make_unique<uint32_t[]>(display.windowWidth * display.windowHeight);

	display.colorBufferTexture = SDL_CreateTexture(
		display.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		display.windowWidth, display.windowHeight);
}

void Update() {
	trianglesToRender.clear();
	// Frame rate limiting
	int timeToWait = display.frameTargetTime - (SDL_GetTicks() - prevFrameTime);
	if (timeToWait > 0 && timeToWait <= display.frameTargetTime) {
		SDL_Delay(timeToWait);
	}
	prevFrameTime = SDL_GetTicks();

	mesh.rotation.x += 0.01;
	//mesh.rotation.y += 0.01;
	//mesh.rotation.z += 0.01;

	for (int i = 0; i < mesh.faces.size(); ++i) {
		Face meshFace = mesh.faces[i]; // indices of our vertices 
		Vector3 faceVertices[3];	   // Set of vertices
		faceVertices[0] = mesh.vertices[meshFace.a - 1];
		faceVertices[1] = mesh.vertices[meshFace.b - 1];
		faceVertices[2] = mesh.vertices[meshFace.c - 1];

		// Apply transformations
		std::vector<Vector3> transformedVertices;
		for (int j = 0; j < 3; ++j) {
			// Apply rotation for fun
			Vector3 transformedPoint = Vec3RotateX(faceVertices[j], mesh.rotation.x);
			//transformedPoint = Vec3RotateY(transformedPoint, mesh.rotation.y);
			//transformedPoint = Vec3RotateZ(transformedPoint, mesh.rotation.z);
			// Translate the vertex away from the camera in z
            //Vector3 transformedPoint = faceVertices[j];
			transformedPoint.z += 5;
			transformedVertices.push_back(transformedPoint);
		}

		// Perform back culling
		Vector3 a = transformedVertices[0];
		Vector3 b = transformedVertices[1];
		Vector3 c = transformedVertices[2];
		// 1. Find vectors B-A and C-A
		Vector3 ab = Subtract(b, a);
		Vector3 ac = Subtract(c, a);
		// 2. Take cross prod and find N
		Vector3 n = Cross(ab, ac);
		Normalize(n);
		// 3. Find the camera ray by subtracting cam pos and point A
		Vector3 camRay = Subtract(camPos, a);
		// 4. Dot product between N and cam ray
		float dot = Dot(n, camRay);
		// 5. If dot < 0 skip the face
        if (dot < 0)
			continue;

		// Project using perspective projection
		Triangle projectedTriangle; // 3 Vertices with 2D coordinates
		for (int j = 0; j < 3; ++j) {
			Vector2 projectedPoint = Project(transformedVertices[j]);
			// Scale and translate the projected points to the middle of the screen
			projectedPoint.x += display.windowWidth / 2;
			projectedPoint.y += display.windowHeight / 2;
			projectedTriangle.points[j] = projectedPoint;
		}

		// Store result
		trianglesToRender.push_back(projectedTriangle);
	}
}

void Render() {
	display.ClearColorBuffer(0xFFFFFFFF);

	for (int i = 0; i < trianglesToRender.size(); ++i) {
		Triangle triangle = trianglesToRender[i];
		display.DrawFilledTriangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			0xFF00FF00);
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
