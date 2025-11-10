
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "sorting.h"
#include "matrix.h"
#include "light.h"

#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>

Display display;
bool isRunning = true;
Vector3 camPos = { 0, 0, 0 };
int prevFrameTime = 0;
int fovFactor = 640;
Mesh mesh;
std::string objPath;
Mat4 perspectiveMat;
std::vector<Triangle> trianglesToRender;
Light light;
uint32_t sourceAmbientColor = 0xFF00FF00;
uint32_t sourceDiffuseColor = 0xFF00FF00;
float sourceAmbientIntensity = 0.2;
float sourceDiffuseIntensity = 0.4;
float specularIntensity = 0.8f;
float shininess = 32.0f; // ����������� ������


void ProcessInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			isRunning = false;
		if (event.key.keysym.sym == SDLK_1)
			display.renderMethod = RenderMethod::RENDER_VERTEX;
		if (event.key.keysym.sym == SDLK_2)
			display.renderMethod = RenderMethod::RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_3)
			display.renderMethod = RenderMethod::RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_4)
			display.renderMethod = RenderMethod::RENDER_FILL_TRIANGLE;
		if (event.key.keysym.sym == SDLK_5)
			display.renderMethod = RenderMethod::RENDER_FILL_TRIANGLE_VERTEX;
		if (event.key.keysym.sym == SDLK_6)
			display.renderMethod = RenderMethod::RENDER_FILL_TRIANGLE_WIRE;
		if (event.key.keysym.sym == SDLK_7)
			display.renderMethod = RenderMethod::RENDER_FILL_TRIANGLE_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_8)
			display.renderMethod = RenderMethod::RENDER_TEXTURED;
		if (event.key.keysym.sym == SDLK_9)
			display.renderMethod = RenderMethod::RENDER_TEXTURED_WIRED;
		if (event.key.keysym.sym == SDLK_c)
			display.cullMethod = CullMethod::CULL_BACKFACE;
		if (event.key.keysym.sym == SDLK_d)
			display.cullMethod = CullMethod::CULL_NONE;
		break;
	}
}

void Setup()
{
#ifdef _WIN32
	// Windows
	objPath = "C:\\Users\\PC\\Desktop\\Pikuma\\assets\\f22.obj";
#elif __APPLE__
	// macOS
	objPath = "/Users/maestro/Desktop/PikumaRender/assets/cube.obj";
#endif
	//mesh.LoadObjFile(objPath);
	mesh.LoadCube();
	meshTexture = (uint32_t*)redBrickTexture;

	display.colorBuffer = std::make_unique<uint32_t[]>(display.windowWidth * display.windowHeight);

	display.colorBufferTexture = SDL_CreateTexture(
		display.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		display.windowWidth, display.windowHeight);
	float fov = 3.141592 / 3.0; // 60 deg
	float aspect = (float)display.windowHeight / (float)display.windowWidth;
	float zNear = 0.1;
	float zFar = 100.0;
	perspectiveMat = Mat4::Perspective(fov, aspect, zNear, zFar);
	light.direction = Vector3(0, 0, 1);
	light.position = Vector3(0, 0, 0);
}

void Update()
{
	// Frame rate limiting
	int timeToWait = display.frameTargetTime - (SDL_GetTicks() - prevFrameTime);
	if (timeToWait > 0 && timeToWait <= display.frameTargetTime)
	{
		SDL_Delay(timeToWait);
	}
	prevFrameTime = SDL_GetTicks();

	// Tranlsation
	//mesh.translation.x += 0.01;
	mesh.translation.z = 6.0;
	Mat4 translationMat = Mat4::Translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	// Create scale matrix
	//mesh.scale.x += 0.2;
	Mat4 scaleMat = Mat4::Scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	// Rotation
	mesh.rotation.x += 0.001;
	mesh.rotation.y += 0.001;
	mesh.rotation.z += 0.001;
	Mat4 rotationMatX = Mat4::RotationAroundXAxis(mesh.rotation.x);
	Mat4 rotationMatY = Mat4::RotationAroundYAxis(mesh.rotation.y);
	Mat4 rotationMatZ = Mat4::RotationAroundZAxis(mesh.rotation.z);

	// Process all triangles
	for (int i = 0; i < mesh.faces.size(); ++i)
	{
		Face meshFace = mesh.faces[i]; // indices of our vertices
		Vector3 faceVertices[3];	   // Set of vertices
		faceVertices[0] = mesh.vertices[meshFace.a - 1];
		faceVertices[1] = mesh.vertices[meshFace.b - 1];
		faceVertices[2] = mesh.vertices[meshFace.c - 1];

		// Apply transformations
		std::vector<Vector4> transformedVertices;
		Mat4 rotationMat = rotationMatY;
		Mat4 worldMatrix = translationMat * rotationMat * scaleMat;
		for (int j = 0; j < 3; ++j)
		{
			Vector4 transformedPoint = Vec3ToVec4(faceVertices[j]);
			transformedPoint = worldMatrix * transformedPoint;
			transformedVertices.push_back(transformedPoint);
		}

		// we need N for flat shading 
		Vector3 a = Vec4ToVec3(transformedVertices[0]);
		Vector3 b = Vec4ToVec3(transformedVertices[1]);
		Vector3 c = Vec4ToVec3(transformedVertices[2]);
		// 1. Find vectors B-A and C-A
		Vector3 ab = Subtract(b, a);
		Vector3 ac = Subtract(c, a);
		// 2. Take cross prod and find N
		Vector3 n = Cross(ab, ac);
		Normalize(n);
		// Perform back culling
		if (display.cullMethod == CullMethod::CULL_BACKFACE)
		{
			// 3. Find the camera ray by subtracting cam pos and point A
			Vector3 camRay = Subtract(camPos, a);
			Normalize(camRay);
			// 4. Dot product between N and cam ray
			float dot = Dot(n, camRay);
			// 5. If dot < 0 skip the face
			if (dot < 0)
				continue;
		}

		// Project using perspective projection
		Triangle projectedTriangle; // 3 Vertices with 2D coordinates
		// Get average depth for each face based on the vertices z-value after trasnformation
		projectedTriangle.averageDepth = (transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3.0;
		// Get projected x and y
		std::vector<Vector2> projectedPoints;
		const int pointsPerTriangle = 3;
		projectedPoints.resize(pointsPerTriangle);
		// Get light 
		//uint32_t triangleColor = meshFace.color;
		uint32_t triangleColor = 0xFF00FF00;
		triangleColor = Light::AmbientPart(triangleColor, sourceAmbientColor, sourceAmbientIntensity);
		Vector3 triangleCenter = {
			(a.x + b.x + c.x) / 3.0f,
			(a.y + b.y + c.y) / 3.0f,
			(a.z + b.z + c.z) / 3.0f
		};
		Vector3 sourceDirection = Subtract(light.position, triangleCenter);
		triangleColor += Light::DiffusePart(n, sourceDirection, triangleColor, sourceDiffuseColor, sourceDiffuseIntensity);
		Vector3 viewDirection = Subtract(camPos, triangleCenter);
		Normalize(viewDirection);
		triangleColor += Light::SpecularPart(n, sourceDirection, viewDirection, triangleColor, sourceDiffuseColor, specularIntensity, shininess);
		for (int j = 0; j < pointsPerTriangle; ++j)
		{
			projectedPoints[j] = Vec4ToVec3(Mat4::MulVec4Projetion(perspectiveMat, transformedVertices[j]));
			// Scale 
			projectedPoints[j].x *= display.windowWidth / 2.0;
			projectedPoints[j].y *= display.windowHeight / 2.0;
			// Translate the projected points to the middle of the screen
			projectedPoints[j].x += display.windowWidth / 2.0;
			projectedPoints[j].y += display.windowHeight / 2.0;
			// take care what our screen coordinates opposite to model
			projectedPoints[j].y = display.windowHeight - projectedPoints[j].y;
			// Store 
			projectedTriangle.points[j] = projectedPoints[j];
			projectedTriangle.color = triangleColor;
		}
		// Texture coordinates
		std::array<Texture, 3> tmpUVArray;
		tmpUVArray[0] = mesh.faces[i].aUV;
		tmpUVArray[1] = mesh.faces[i].bUV;
		tmpUVArray[2] = mesh.faces[i].cUV;
		projectedTriangle.texCoords = tmpUVArray;

		// Store result
		trianglesToRender.push_back(projectedTriangle);
	}

	// Sort triangles to use painter algorithm
	BubleSort(trianglesToRender);
}

void Render()
{
	display.ClearColorBuffer(0xFF000000);

	for (int i = 0; i < trianglesToRender.size(); ++i)
	{
		Triangle triangle = trianglesToRender[i];

		// triangle vertex points
		if (display.renderMethod == RenderMethod::RENDER_VERTEX ||
			display.renderMethod == RenderMethod::RENDER_WIRE_VERTEX ||
			display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_VERTEX ||
			display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE_VERTEX)
		{
			uint32_t color = triangle.color;
			display.DrawRectangle(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, color);
			display.DrawRectangle(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, color);
			display.DrawRectangle(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, color);
		}

		// triangle wireframe
		if (display.renderMethod == RenderMethod::RENDER_WIRE ||
			display.renderMethod == RenderMethod::RENDER_WIRE_VERTEX ||
			display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE_VERTEX ||
			display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE ||
			display.renderMethod == RenderMethod::RENDER_TEXTURED_WIRED)
		{
			display.DrawTriangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color);
		}

		// filled triangle
		if (display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE ||
			display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE ||
			display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_WIRE_VERTEX ||
			display.renderMethod == RenderMethod::RENDER_FILL_TRIANGLE_VERTEX)
		{
			display.DrawFilledTriangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color);
		}

		// Textured triangle
		if (display.renderMethod == RenderMethod::RENDER_TEXTURED ||
			display.renderMethod == RenderMethod::RENDER_TEXTURED_WIRED)
		{
			display.DrawTexturedTriangle(
				triangle.points[0].x, triangle.points[0].y, triangle.texCoords[0].u, triangle.texCoords[0].v,
				triangle.points[1].x, triangle.points[1].y, triangle.texCoords[1].u, triangle.texCoords[2].v,
				triangle.points[2].x, triangle.points[2].y, triangle.texCoords[1].u, triangle.texCoords[2].v,
				meshTexture
			);
		}
	}
	trianglesToRender.clear();
	display.RenderColorBuffer();

	SDL_RenderPresent(display.renderer);
}

int main(int argc, char* argv[])
{

	isRunning = display.InitWindow();

	Setup();

	SDL_Event event;

	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

	display.DestroyWindow();

	std::cout << "Application closed successfully!" << std::endl;
	return 0;
}
