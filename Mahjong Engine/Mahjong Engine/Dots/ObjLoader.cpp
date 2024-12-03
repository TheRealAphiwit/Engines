#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <glm.hpp>  
#include <algorithm>
#include "Mesh.h"

bool DotsRendering::LoadOBJ(const std::string& filename, ObjData& outData)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open OBJ file: " << filename << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream ss(line); 
		std::string prefix;
		ss >> prefix;

		if (prefix == "v") // Vertices
		{
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			outData.vertices.push_back(vertex);
		}
		else if (prefix == "vn") //Vertex normals
		{
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			outData.normals.push_back(normal);
		}
		else if (prefix == "vt") // Vertex texture
		{
			glm::vec2 textCoord;
			ss >> textCoord.x >> textCoord.y;
			outData.textCoords.push_back(textCoord);
		}
		else if (prefix == "f") // Face definitions
		{
			unsigned int vIndex[3], tIndex[3], nIndex[3];
			char slash;
			for (int i = 0; i < 3; ++i)
			{
				ss >> vIndex[i] >> slash >> tIndex[i] >> slash >> nIndex[i];
				outData.indices.push_back(vIndex[i] - 1);
			}
		}
	}

	file.close();
    return true;
}

Mesh* DotsRendering::LoadObjMesh(const std::string& filename)
{
	ObjData objData;
	if (LoadOBJ(filename, objData))
	{
		return new Mesh(objData);
	}
	else
	{
		std::cout << "Could not load mesh" << filename << std::endl;
		return nullptr;
	}
}