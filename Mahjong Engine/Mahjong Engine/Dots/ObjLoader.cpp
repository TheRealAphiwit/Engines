#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
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

bool DotsRendering::SerializeObjData(const std::string& filename, const ObjData& data)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open OBJ file: " << filename << std::endl;
		return false;
	}

	// Metadata structure declaration
	int numVertices = data.vertices.size();
	int numTextCoords = data.textCoords.size();
	int numNormals = data.normals.size();
	int numIndices = data.indices.size();

	//Tells compiler to write the data as a binary file
	file.write(reinterpret_cast<const char*>(&numVertices), sizeof(int));
	file.write(reinterpret_cast<const char*>(&numTextCoords), sizeof(int));
	file.write(reinterpret_cast<const char*>(&numNormals), sizeof(int));
	file.write(reinterpret_cast<const char*>(&numIndices), sizeof(int));

	// Write the actual data
	file.write(reinterpret_cast<const char*>(data.vertices.data()), numVertices * sizeof(glm::vec3));
	file.write(reinterpret_cast<const char*>(data.textCoords.data()), numTextCoords * sizeof(glm::vec2));
	file.write(reinterpret_cast<const char*>(data.normals.data()), numNormals * sizeof(glm::vec3));
	file.write(reinterpret_cast<const char*>(data.indices.data()), numIndices * sizeof(unsigned int));

	file.close();
	return true;
}

bool DotsRendering::DeserializeObjData(const std::string& filename, ObjData& outData)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open OBJ file: " << filename << std::endl;
		return false;
	}

	// Read up metadata and prep data
	int numVertices, numTextCoords, numNormals, numIndices;
	file.read(reinterpret_cast<char*>(&numVertices), sizeof(int));
	file.read(reinterpret_cast<char*>(&numTextCoords), sizeof(int));
	file.read(reinterpret_cast<char*>(&numNormals), sizeof(int));
	file.read(reinterpret_cast<char*>(&numIndices), sizeof(int));

	// Resize the vectors (to accomodate data)
	outData.vertices.resize(numVertices);
	outData.textCoords.resize(numTextCoords);
	outData.normals.resize(numNormals);
	outData.indices.resize(numIndices);

	// Read and assign the actual data
	file.read(reinterpret_cast<char*>(outData.vertices.data()), numVertices * sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(outData.textCoords.data()), numTextCoords * sizeof(glm::vec2));
	file.read(reinterpret_cast<char*>(outData.normals.data()), numNormals * sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(outData.indices.data()), numIndices * sizeof(unsigned int));

	file.close();
	return true;
}
