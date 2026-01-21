#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "Mesh.h"

struct VertexKey
{
	unsigned int vIdx, vtIdx, nIdx;

	bool operator<(const VertexKey& other) const {
		if (vIdx != other.vIdx) return vIdx < other.vIdx;
		if (vtIdx != other.vtIdx) return vtIdx < other.vtIdx;
		return nIdx < other.nIdx;
	}
};

bool DotsRendering::LoadOBJ(const std::string& filename, ObjData& outData)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open OBJ file: " << filename << std::endl;
		return false;
	}

	// Temporary storage for RAW file data - part of the update for objloader to be able to handle faces of .objs
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	std::map<VertexKey, unsigned int> uniqueVertices;

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
			tempVertices.push_back(vertex);
		}
		else if (prefix == "vt") // Vertex texture
		{
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			tempUVs.push_back(uv);
		}
		else if (prefix == "vn") //Vertex normals
		{
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			tempNormals.push_back(normal);
		}
		else if (prefix == "f") // Face definitions
		{
			std::string vertexStr;
			std::vector<VertexKey> faceVertices;

			while (ss >> vertexStr)
			{
				std::replace(vertexStr.begin(), vertexStr.end(), '/', ' ');
				std::istringstream vss(vertexStr);

				unsigned int vIdx = 0, vtIdx = 0, nIdx = 0;
				vss >> vIdx;

				// Handle texture coords (might be missing)
				if (vss.peek() == ' ') vss.ignore();
				if (vss.peek() != ' ' && vss.peek() != EOF) vss >> vtIdx;

				// Handle normals (might be missing)
				if (vss.peek() == ' ') vss.ignore();
				if (vss.peek() != EOF) vss >> nIdx;

				// Adjust to 0-based index
				VertexKey key = { vIdx - 1, (vtIdx > 0 ? vtIdx - 1 : 0), (nIdx > 0 ? nIdx - 1 : 0) };
				faceVertices.push_back(key);
			}

			for (size_t i = 1; i < faceVertices.size() - 1; i++)
			{
				VertexKey keys[3] = { faceVertices[0], faceVertices[i], faceVertices[i + 1] };

				for (int k = 0; k < 3; k++)
				{
					VertexKey currentKey = keys[k];

					if (uniqueVertices.find(currentKey) == uniqueVertices.end())
					{
						unsigned int newIndex = static_cast<unsigned int>(outData.vertices.size());
						uniqueVertices[currentKey] = newIndex;

						// Position
						outData.vertices.push_back(tempVertices[currentKey.vIdx]);

						// Texture 
						if (!tempUVs.empty() && currentKey.vtIdx < tempUVs.size())
							outData.textCoords.push_back(tempUVs[currentKey.vtIdx]);
						else
							outData.textCoords.push_back(glm::vec2(0.0f, 0.0f));

						// Normal 
						if (!tempNormals.empty() && currentKey.nIdx < tempNormals.size())
							outData.normals.push_back(tempNormals[currentKey.nIdx]);
						else
							outData.normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // Default Up

						outData.indices.push_back(newIndex);
					}
					else
					{
						// Reuse existing vertex
						outData.indices.push_back(uniqueVertices[currentKey]);
					}
				}
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
