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
};

