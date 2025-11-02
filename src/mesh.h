#pragma once

#include "vector.h"
#include "triangle.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>

struct Mesh {
	std::vector<Vector3> vertices;
	std::vector<Face> faces;
	Vector3 rotation{ 0, 0, 0 };     // angles in x,y,z (Euler angles)
	Vector3 scale{ 1, 1, 1 };		 // scale with x,y, z values
	Vector3 translation{ 0, 0, 0 };  // translation with x,y,z values

	void LoadObjFile(const std::string& path) {
		std::ifstream file(path);
		if (!file.is_open())
			throw std::runtime_error("File does not exist: " + path);
		std::string str;
		while (std::getline(file, str)) {
			if (str[0] == 'v' && str[1] == ' ') {
				std::string tmpStr = str.substr(1);
				std::istringstream iss(tmpStr);
				float x, y, z;
				iss >> x >> y >> z;
				Vector3 tmp{ x, y, z };
				vertices.push_back(tmp);
			}
			else if (str[0] == 'f') {
				std::string tmpStr = str.substr(1);
				std::istringstream iss(tmpStr);
				Face face;
				int first, second, third;
				char slash;
				iss >> first >> slash >> second >> slash >> third;
				face.a = first;
				iss >> first >> slash >> second >> slash >> third;
				face.b = first;
				iss >> first >> slash >> second >> slash >> third;
				face.c = first;
				face.color =
					0xFF000000 |                     // Alpha
					((std::rand() % 256) << 16) |    // Red: 0-255
					((std::rand() % 256) << 8) |     // Green: 0-255  
					(std::rand() % 256);             // Blue: 0-255
				faces.push_back(face);
			}
		}
	}

	void LoadCube() {
		// Define cube vertices
		std::vector<Vector3> cube_vertices = {
			{ -1, -1, -1 }, // 1
			{ -1,  1, -1 }, // 2  
			{  1,  1, -1 }, // 3
			{  1, -1, -1 }, // 4
			{  1,  1,  1 }, // 5
			{  1, -1,  1 }, // 6
			{ -1,  1,  1 }, // 7
			{ -1, -1,  1 }  // 8
		};

		// Define cube faces
		std::vector<Face> cube_faces = {
			// front
			{ 1, 2, 3, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFFFF },
			{ 1, 3, 4, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFFFF },
			// right
			{ 4, 3, 5, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFFFF },
			{ 4, 5, 6, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFFFF },
			// back
			{ 6, 5, 7, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFFFF },
			{ 6, 7, 8, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFFFF },
			// left
			{ 8, 7, 2, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFFFF },
			{ 8, 2, 1, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFFFF },
			// top
			{ 2, 7, 5, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFFFF },
			{ 2, 5, 3, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFFFF },
			// bottom
			{ 6, 8, 1, {0, 0}, {0, 1}, {1, 1}, 0xFFFFFFFF },
			{ 6, 1, 4, {0, 0}, {1, 1}, {1, 0}, 0xFFFFFFFF }
		};

		// Load into mesh
		vertices = cube_vertices;
		faces = cube_faces;
	}
};

