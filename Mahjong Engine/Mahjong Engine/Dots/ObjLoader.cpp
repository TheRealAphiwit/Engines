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
			std::vector<unsigned int> vIndices, tIndices, nIndices;
			std::string vertexData;

			while (ss >> vertexData)
			{
				std::replace(vertexData.begin(), vertexData.end(), '/', ' '); // Replace '/' with space
				std::istringstream vss(vertexData);
				unsigned int vIdx = 0, tIdx = 0, nIdx = 0;
				vss >> vIdx;

				if (vss.peek() == ' ') vss.ignore(); // Skip space
				if (vss >> tIdx) {}
				else tIdx = 0;  // Optional texture coordinate
				if (vss.peek() == ' ') vss.ignore();
				if (vss >> nIdx) {}
				else nIdx = 0;  // Optional normal

				vIndices.push_back(vIdx - 1); // Convert to 0-based indexing
				if (tIdx > 0) tIndices.push_back(tIdx - 1);
				if (nIdx > 0) nIndices.push_back(nIdx - 1);
			}

			// Triangulate if more than 3 vertices in a face
			for (size_t i = 1; i + 1 < vIndices.size(); i++)
			{
				outData.indices.push_back(vIndices[0]);
				outData.indices.push_back(vIndices[i]);
				outData.indices.push_back(vIndices[i + 1]);
			}
		}
	}

	// Print out the data
	std::cout << "(ObjLoader) OBJ file: " << filename << std::endl;
	std::cout << "Vertices: " << outData.vertices.size() << std::endl;
	std::cout << "Normals: " << outData.normals.size() << std::endl;
	std::cout << "TextCoords: " << outData.textCoords.size() << std::endl;
	std::cout << "Indices: " << outData.indices.size() << std::endl;

	file.close();
    return true;
}

// Can be used for both .obj and .bin files
Mesh* DotsRendering::LoadObjMesh(const std::string& filename)
{
	ObjData objData;
	std::string binaryPath = "../Assets/Meshes/Binary/" + filename + ".bin";
	std::string objPath = "../Assets/Meshes/Obj/" + filename + ".obj";

	// Currently don't work: flagmesh is still empty
	try
	{
		// First: Try loading binary file
		if (DeserializeObjData(binaryPath, objData))
		{return new Mesh(objData);} 
		else 
		{std::cout << "Failed to load binary file: " << binaryPath << std::endl;}

		// Second: Try loading OBJ file
		if (LoadOBJ(objPath, objData))
		{
			// Serialize the data to binary file
			std::cout << "Serializing OBJ data to binary file" << std::endl;
			SerializeObjData(binaryPath, objData);
			return new Mesh(objData);
		} 
		else {std::cout << "Failed to load OBJ file: " << objPath << std::endl;}
	}
	catch (const std::exception&)
	{
		std::cout << "Failed to load mesh" << filename << std::endl;
		return nullptr;
	}
}

bool DotsRendering::SerializeObjData(const std::string& filename, const ObjData& data)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to SERIALIZE file: " << filename << std::endl;
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
		std::cerr << "Failed to DESERIALIZE obj data: " << filename << std::endl;
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
