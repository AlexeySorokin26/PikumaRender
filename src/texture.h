#pragma once

#include <cstdint>

struct Texture {
	float u;
	float v;
};

extern int textureWidth; // Extern here mearns Declaration only - no memory allocated
extern int textureHeight;
extern const uint8_t redBrickTexture[];
extern uint32_t* meshTexture;