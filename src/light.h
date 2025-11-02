#pragma once

#include "vector.h"

struct Light {
	Light(const Vector3 d = Vector3(0, 0, 0), const Vector3 p = Vector3(0, 0, 0)) : direction(d), position(p) {
	}
	Vector3 direction;
	Vector3 position;

	static uint32_t SpecularPart(Vector3 normal, Vector3 sourceDirection, Vector3 viewDirection,
		uint32_t originalColor, uint32_t sourceColor, float sourceIntensity, float shininess) {
		sourceIntensity = std::min(std::max(sourceIntensity, 0.0f), 1.0f);
		shininess = std::max(shininess, 1.0f);

		// Нормализуем векторы
		Normalize(normal);
		Normalize(sourceDirection);
		Normalize(viewDirection);

		// Вычисляем отраженный свет
		float dotNL = Dot(normal, sourceDirection);
		Vector3 reflectDirection = {
			2 * dotNL * normal.x - sourceDirection.x,
			2 * dotNL * normal.y - sourceDirection.y,
			2 * dotNL * normal.z - sourceDirection.z
		};
		Normalize(reflectDirection);

		// Вычисляем dot product между отраженным светом и направлением взгляда
		float specularFactor = Dot(reflectDirection, viewDirection);
		specularFactor = std::max(specularFactor, 0.0f); // Не может быть отрицательным

		// Применяем блеск (shininess)
		specularFactor = powf(specularFactor, shininess);

		// Извлекаем компоненты цвета источника света
		uint32_t rLight = (sourceColor & 0x00FF0000) >> 16;
		uint32_t gLight = (sourceColor & 0x0000FF00) >> 8;
		uint32_t bLight = (sourceColor & 0x000000FF);

		// Вычисляем specular компоненты
		float rResult = (rLight / 255.0f) * sourceIntensity * specularFactor;
		float gResult = (gLight / 255.0f) * sourceIntensity * specularFactor;
		float bResult = (bLight / 255.0f) * sourceIntensity * specularFactor;

		// Ограничиваем и конвертируем обратно
		uint32_t r_final = static_cast<uint32_t>(std::min(rResult, 1.0f) * 255);
		uint32_t g_final = static_cast<uint32_t>(std::min(gResult, 1.0f) * 255);
		uint32_t b_final = static_cast<uint32_t>(std::min(bResult, 1.0f) * 255);

		return (r_final << 16) | (g_final << 8) | b_final;
	}

	static uint32_t DiffusePart(Vector3 normal, Vector3 sourceDirection, uint32_t originalColor, uint32_t sourceColor, float sourceIntensity) {
		sourceIntensity = std::min(std::max(sourceIntensity, 0.0f), 1.0f);

		// Извлекаем компоненты исходного цвета объекта
		uint32_t a = (originalColor & 0xFF000000);
		uint32_t rObj = (originalColor & 0x00FF0000) >> 16;
		uint32_t gObj = (originalColor & 0x0000FF00) >> 8;
		uint32_t bObj = (originalColor & 0x000000FF);

		// Извлекаем компоненты цвета источника света
		uint32_t rLight = (sourceColor & 0x00FF0000) >> 16;
		uint32_t gLight = (sourceColor & 0x0000FF00) >> 8;
		uint32_t bLight = (sourceColor & 0x000000FF);

		Normalize(normal);
		Normalize(sourceDirection);
		float dotNL = Dot(normal, sourceDirection);

		// Нормализуем к [0,1] и применяем освещение
		float rResult = (rObj / 255.0f) * (rLight / 255.0f) * sourceIntensity * dotNL;
		float gResult = (gObj / 255.0f) * (gLight / 255.0f) * sourceIntensity * dotNL;
		float bResult = (bObj / 255.0f) * (bLight / 255.0f) * sourceIntensity * dotNL;

		// Ограничиваем и конвертируем обратно
		uint32_t r_final = static_cast<uint32_t>(std::min(rResult, 1.0f) * 255);
		uint32_t g_final = static_cast<uint32_t>(std::min(gResult, 1.0f) * 255);
		uint32_t b_final = static_cast<uint32_t>(std::min(bResult, 1.0f) * 255);

		return a | (r_final << 16) | (g_final << 8) | b_final;
	}

	static uint32_t AmbientPart(uint32_t originalColor, uint32_t sourceColor, float sourceIntensity) {
		sourceIntensity = std::min(std::max(sourceIntensity, 0.0f), 1.0f);

		// Извлекаем компоненты исходного цвета объекта
		uint32_t a = (originalColor & 0xFF000000);
		uint32_t rObj = (originalColor & 0x00FF0000) >> 16;
		uint32_t gObj = (originalColor & 0x0000FF00) >> 8;
		uint32_t bObj = (originalColor & 0x000000FF);

		// Извлекаем компоненты цвета источника света
		uint32_t rLight = (sourceColor & 0x00FF0000) >> 16;
		uint32_t gLight = (sourceColor & 0x0000FF00) >> 8;
		uint32_t bLight = (sourceColor & 0x000000FF);

		// Нормализуем к [0,1] и применяем освещение
		float rResult = (rObj / 255.0f) * (rLight / 255.0f) * sourceIntensity;
		float gResult = (gObj / 255.0f) * (gLight / 255.0f) * sourceIntensity;
		float bResult = (bObj / 255.0f) * (bLight / 255.0f) * sourceIntensity;

		// Ограничиваем и конвертируем обратно
		uint32_t rFinal = static_cast<uint32_t>(std::min(rResult, 1.0f) * 255);
		uint32_t gFinal = static_cast<uint32_t>(std::min(gResult, 1.0f) * 255);
		uint32_t bFinal = static_cast<uint32_t>(std::min(bResult, 1.0f) * 255);

		return a | (rFinal << 16) | (gFinal << 8) | bFinal;
	}

	static uint32_t LightApplyIntensity(uint32_t originalColor, float percentageFactor) {
		if (percentageFactor < 0) percentageFactor = 0;
		if (percentageFactor > 1) percentageFactor = 1;

		uint32_t a = (originalColor & 0xFF000000);
		uint32_t r = (originalColor & 0x00FF0000) >> 16;;
		uint32_t g = (originalColor & 0x0000FF00) >> 8;
		uint32_t b = (originalColor & 0x000000FF);

		r = static_cast<uint32_t>(r * percentageFactor);
		g = static_cast<uint32_t>(g * percentageFactor);
		b = static_cast<uint32_t>(b * percentageFactor);

		uint32_t newColor = a | (r << 16) | (g << 8) | b;

		return newColor;
	}
};

