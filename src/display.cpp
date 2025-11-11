#include "display.h"
#include "texture.h"

bool Display::InitWindow()
{
	// Init sdl
	{
		std::cout << "Initializing SDL..." << std::endl;

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
			return false;
		}
	}

	//// Use SDL to query what is the fullscreen max
	//{
	//	SDL_DisplayMode displayMode;
	//	SDL_GetCurrentDisplayMode(0, &displayMode);
	//	windowWidth = displayMode.w;
	//	windowHeight = displayMode.h;
	//}

	// Creating sdl window
	{
		std::cout << "Creating SDL window..." << std::endl;

		window = SDL_CreateWindow("SDL Example",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);

		if (!window)
		{
			std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
			SDL_Quit();
			return false;
		}
	}

	// Creating sld renderer
	{
		renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}

	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void Display::DrawRectangle(int x, int y, int w, int h, uint32_t color)
{
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			int currentX = x + i;
			int currentY = y + j;
			DrawPixel(currentX, currentY, color);
		}
	}
}

void Display::DrawGrid()
{
	uint32_t color = 0xFFFFFFFF;
	for (int y = 0; y < windowHeight; ++y)
	{
		for (int x = 0; x < windowWidth; ++x)
		{
			if (y % 10 == 0 || x % 10 == 0)
				colorBuffer[windowWidth * y + x] = color;
		}
	}
}

void Display::RenderColorBuffer()
{
	SDL_UpdateTexture(
		colorBufferTexture, NULL, colorBuffer.get(), windowWidth * sizeof(uint32_t)); // Source, if we need to take only cut of it, source of data, pitch just how many pixels we have in a row
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);						  // last 2 args if we want just part of data
}

void Display::ClearColorBuffer(uint32_t color)
{
	for (int y = 0; y < windowHeight; ++y)
	{
		for (int x = 0; x < windowWidth; ++x)
		{
			colorBuffer[windowWidth * y + x] = color;
		}
	}
}

void Display::DestroyWindow()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::DrawPixel(int x, int y, uint32_t color)
{
	if (x >= 0 && x < windowWidth && y >= 0 && y < windowHeight)
		colorBuffer[y * windowWidth + x] = color;
}

void Display::DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
	int deltaX = x1 - x0;
	int deltaY = y1 - y0;

	// Depending on if deltaX is > deltaY or opposite
	int longestSideLength = (std::abs(deltaX) >= std::abs(deltaY)) ? std::abs(deltaX) : std::abs(deltaY);

	// Handle zero-length lines
	if (longestSideLength == 0)
	{
		DrawPixel(x0, y0, color);
		return;
	}

	float xInc = deltaX / static_cast<float>(longestSideLength);
	float yInc = deltaY / static_cast<float>(longestSideLength);

	float currentX = x0;
	float currentY = y0;

	// Include the endpoint by using <=
	for (int i = 0; i <= longestSideLength; ++i)
	{
		DrawPixel(std::round(currentX), std::round(currentY), color);
		currentX += xInc;
		currentY += yInc;
	}
}

void Display::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	DrawLine(x0, y0, x1, y1, color);
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x0, y0, color);
}
void Display::DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// Sort vertices by y-coordinate (y0 <= y1 <= y2)
	if (y0 > y1)
	{
		std::swap(y0, y1);
		std::swap(x0, x1);
	}
	if (y0 > y2)
	{
		std::swap(y0, y2);
		std::swap(x0, x2);
	}
	if (y1 > y2)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
	}

	// Check for degenerate triangles
	if (y0 == y2)
		return;

	// Calculate Mx using triangle similarity
	int My = y1;
	int Mx = (x2 - x0) * (y1 - y0) / (float)(y2 - y0) + x0;

	// Draw both parts
	if (y0 == y1)
	{
		// Flat top triangle
		FillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y1 == y2)
	{
		// Flat bottom triangle
		FillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
	}
	else
	{
		// General case - split into flat bottom and flat top
		FillFlatBottomTriangle(x0, y0, x1, y1, Mx, y1, color);
		FillFlatTopTriangle(x1, y1, Mx, y1, x2, y2, color);
	}
}

void Display::FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// Avoid division by zero
	if (y1 == y0)
		return;

	float slope1 = (float)(x1 - x0) / (float)(y1 - y0);
	float slope2 = (float)(x2 - x0) / (float)(y2 - y0);

	float xStart = (float)x0;
	float xEnd = (float)x0;

	for (int y = y0; y <= y1; y++)
	{
		DrawLine((int)std::round(xStart), y, (int)std::round(xEnd), y, color);
		xStart += slope1;
		xEnd += slope2;
	}
}

void Display::FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// Avoid division by zero
	if (y2 == y0)
		return;

	float slope1 = (float)(x2 - x0) / (float)(y2 - y0);
	float slope2 = (float)(x2 - x1) / (float)(y2 - y1);

	float xStart = (float)x2;
	float xEnd = (float)x2;

	for (int y = y2; y >= y0; y--)
	{
		DrawLine((int)std::round(xStart), y, (int)std::round(xEnd), y, color);
		xStart -= slope1;
		xEnd -= slope2;
	}
}

void Display::DrawTexturedTriangle(
	int x0, int y0, float u0, float v0,
	int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	uint32_t* texture)
{
	// Sort vertices by y-coordinate (y0 <= y1 <= y2)
	if (y0 > y1)
	{
		std::swap(y0, y1);
		std::swap(x0, x1);
		std::swap(u0, u1);
		std::swap(v0, v1);
	}
	if (y0 > y2)
	{
		std::swap(y0, y2);
		std::swap(x0, x2);
		std::swap(u0, u2);
		std::swap(v0, v2);
	}
	if (y1 > y2)
	{
		std::swap(y1, y2);
		std::swap(x1, x2);
		std::swap(u1, u2);
		std::swap(v1, v2);
	}

	// Check for degenerate triangles
	if (y0 == y2)
		return;

	// Create vector points 
	Vector2 pointA = { float(x0), float(y0) };
	Vector2 pointB = { float(x1), float(y1) };
	Vector2 pointC = { float(x2), float(y2) };

	// Draw the flat-bottom triangle
	float invSlopeLeft = 0;
	float invSlopeRight = 0;

	if (y1 - y0 != 0)
		invSlopeLeft = float(x1 - x0) / (y1 - y0);
	if (y2 - y0 != 0)
		invSlopeRight = float(x2 - x0) / (y2 - y0);

	if (y1 - y0 != 0)
	{
		for (int y = y0; y <= y1; ++y)
		{
			int xStart = x0 + (y - y0) * invSlopeLeft; // delta y * tan
			int xEnd = x0 + (y - y0) * invSlopeRight;  // delta y * tan

			if (xEnd < xStart) std::swap(xEnd, xStart);
			for (int x = xStart; x < xEnd; ++x)
				DrawTexel(x, y, texture, pointA, pointB, pointC, u0, v0, u1, v1, u2, v2);
		}
	}

	// Draw the flat-top triangle

	if (y2 - y1 != 0)
	{
		float invSlopeLeft = float(x2 - x0) / (y2 - y0);
		float invSlopeRight = float(x2 - x1) / (y2 - y1);

		for (int y = y1; y <= y2; ++y)
		{
			int xStart = x0 + (y - y0) * invSlopeLeft;
			int xEnd = x1 + (y - y1) * invSlopeRight;

			if (xEnd < xStart) std::swap(xEnd, xStart);
			for (int x = xStart; x <= xEnd; ++x)
				DrawTexel(x, y, texture, pointA, pointB, pointC, u0, v0, u1, v1, u2, v2);
		}
	}
}

Vector3 Display::BarycentricWeights(Vector2 a, Vector2 b, Vector2 c, Vector2 p) {
	// We have 2D vectors actually that's why it's called cross product 
	// [a.x] [b.x]
	// [a.y] [b.y]
	// cross multiplication
	Vector2 ac = c - a;
	Vector2 ab = b - a;
	Vector2 pc = c - p;
	Vector2 pb = b - p;
	Vector2 ap = p - a;

	// areas
	float areaABC = ac.x * ab.y - ac.y * ab.x;
	float alpha = (pc.x * pb.y - pc.y * pb.x) / areaABC;
	float beta = (ac.x * ap.y - ac.y * ap.x) / areaABC;
	float gamma = 1 - alpha - beta;

	return Vector3(alpha, beta, gamma);
}

void Display::DrawTexel(int x, int y, uint32_t* texture, Vector2 pointA, Vector2 pointB, Vector2 pointC,
	float u0, float v0, float u1, float v1, float u2, float v2) {
	Vector2 pointP = { float(x),float(y) };
	Vector3 weights = BarycentricWeights(pointA, pointB, pointC, pointP);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolatedU = u0 * alpha + u1 * beta + u2 * gamma;
	float interpolatedV = v0 * alpha + v1 * beta + v2 * gamma;

	int texX = abs(int(interpolatedU * textureWidth)) % textureWidth;
	int texY = abs(int(interpolatedV * textureHeight)) % textureHeight;

	DrawPixel(x, y, texture[texX + texY * textureWidth]);
}