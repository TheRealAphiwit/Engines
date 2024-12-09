#pragma once
#include <vector>
#include <glm.hpp>
#include <string>

class Mesh;

namespace DotsRendering
{
	struct ObjData
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> textCoords;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
	};

	bool LoadOBJ(const std::string& filename, ObjData& outData);
	Mesh* LoadObjMesh(const std::string& filename);

	bool SerializeObjData(const std::string& filename, const ObjData& data);
	bool DeserializeObjData(const std::string& filename, ObjData& outData);

	#pragma region Goal
	// Implement binary file creation to work alongside serializaion and deserialization
	// Check also before loading and creating a binary file if the file already exists
	#pragma endregion
}