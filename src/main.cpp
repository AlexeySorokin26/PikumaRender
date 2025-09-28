#include <SDL.h>
#include <iostream>
#include <memory>

bool isRunning = true;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Texture* colorBufferTexture = nullptr;
std::unique_ptr<uint32_t[]> colorBuffer = nullptr;

int windowWidth = 800;
int windowHeight = 600;

bool InitWindow() {
	// Init sdl
	{
		std::cout << "Initializing SDL..." << std::endl;

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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

		if (!window) {
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

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void Setup() {
	colorBuffer = std::make_unique<uint32_t[]>(windowWidth * windowHeight);

	colorBufferTexture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);
}

void DestroyWindow() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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

void RenderColorBuffer() {
	SDL_UpdateTexture(
		colorBufferTexture, NULL, colorBuffer.get(), windowWidth * sizeof(uint32_t)); // Source, if we need to take only cut of it, source of data, pitch just how many pixels we have in a row
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL); // last 2 args if we want just part of data
}
void Update() {

}

void ClearColorBuffer(uint32_t color) {
	for (int y = 0; y < windowHeight; ++y) {
		for (int x = 0; x < windowWidth; ++x) {
			colorBuffer[windowWidth * y + x] = color;
		}
	}
}

void DrawGrid() {
	uint32_t color = 0xFFFFFFFF;
	for (int y = 0; y < windowHeight; ++y) {
		for (int x = 0; x < windowWidth; ++x) {
			if (y % 10 == 0 || x % 10 == 0)
				colorBuffer[windowWidth * y + x] = color;
		}
	}
}

void DrawRectangle(int x, int y, int w, int h, uint32_t color) {
	int64_t startPos = windowWidth * y + x;
	for (int i = 0; i < h; ++i) {
		int64_t rowStart = startPos + windowWidth * i;
		for (int j = 0; j < w; ++j) {
			colorBuffer[rowStart + j] = color;
		}
	}
}

void Render() {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	ClearColorBuffer(0xFF00FF00);
	DrawGrid();
	DrawRectangle(100, 50, 100, 100, 0xFFFF00FF);
	RenderColorBuffer();

	SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {

	isRunning = InitWindow();

	Setup();

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