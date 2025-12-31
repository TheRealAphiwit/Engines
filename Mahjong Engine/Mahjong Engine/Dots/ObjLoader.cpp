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

	std::vector<glm::vec3> tempPositions;
	std::vector<glm::vec3> tempNormals;
	std::vector<glm::vec2> tempUVs;

	std::vector<Vertex> finalVertices;
	std::vector<unsigned int> finalIndices;

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string prefix;
		ss >> prefix;

		if (prefix == "v")
		{
			glm::vec3 v;
			ss >> v.x >> v.y >> v.z;
			tempPositions.push_back(v);
		}
		else if (prefix == "vn")
		{
			glm::vec3 n;
			ss >> n.x >> n.y >> n.z;
			tempNormals.push_back(n);
		}
		else if (prefix == "vt")
		{
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			uv.y = 1.0f - uv.y; 
			tempUVs.push_back(uv);
		}
		else if (prefix == "f")
		{
			std::vector<std::string> faceVerts;
			std::string v;
			while (ss >> v)
				faceVerts.push_back(v);

			// triangulate 
			for (size_t i = 1; i + 1 < faceVerts.size(); ++i)
			{
				std::string tri[3] =
				{
					faceVerts[0],
					faceVerts[i],
					faceVerts[i + 1]
				};

				for (int k = 0; k < 3; ++k)
				{
					std::stringstream vss(tri[k]);
					std::string token;
					int indices[3] = { -1, -1, -1 };
					int idx = 0;

					while (std::getline(vss, token, '/'))
					{
						if (!token.empty())
							indices[idx] = std::stoi(token) - 1;
						idx++;
					}

					Vertex vert{};
					vert.Position = tempPositions[indices[0]];

					if (indices[1] >= 0 && indices[1] < (int)tempUVs.size())
						vert.TexCoords = tempUVs[indices[1]];
					else
						vert.TexCoords = glm::vec2(0.0f);

					if (indices[2] >= 0 && indices[2] < (int)tempNormals.size())
						vert.Normal = tempNormals[indices[2]];
					else
						vert.Normal = glm::vec3(0.0f, 1.0f, 0.0f);

					finalVertices.push_back(vert);
					finalIndices.push_back((unsigned int)finalVertices.size() - 1);
				}
			}
		}
	}

	file.close();

	outData.vertices.clear();
	outData.textCoords.clear();
	outData.normals.clear();
	outData.indices.clear();

	outData.vertices.reserve(finalVertices.size());
	outData.indices = finalIndices;

	// FLATTEN Vertex -> ObjData
	for (const Vertex& v : finalVertices)
	{
		outData.vertices.push_back(v.Position);
		outData.textCoords.push_back(v.TexCoords);
		outData.normals.push_back(v.Normal);
	}

	std::cout << "(OBJ) Loaded "
		<< finalVertices.size() << " vertices, "
		<< finalIndices.size() << " indices\n";

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
		/*if (DeserializeObjData(binaryPath, objData))
		{return new Mesh(objData);} 
		else 
		{std::cout << "Failed to load binary file: " << binaryPath << std::endl;}*/

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
